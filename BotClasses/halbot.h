#ifndef HALBOT_H
#define HALBOT_H

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  MegaHAL (3)   Jason Hutchens (Aus)

 MegaHAL     (from: http://ciips.ee.uwa.edu.au/~hutch/puzzles/roshambo/)

 MegaHAL, named in honour of a conversation simulator of mine, was my entry
 into the First International RoShamBo Programming Competition, which was
 conducted by Darse Billings. MegaHAL came third in the competition, behind
 the excellent Iocaine Powder of Dan Egnor, and Phasenbott by Jakob
 Mandelson. This web page is a brief attempt to explain how the MegaHAL
 algorithm works.

 Source Code

 Please feel free to download the source code to the MegaHAL algorithm. To
 compile it with Darse's tournament program (available from the competition
 home page), I recommend that you modify the tournament program by adding an
 external declaration to the halbot() function, and then linking the code as
 follows:

 gcc -o roshambo roshambo.c megahal.c

 I have also written a simple program which allows a human being to play
 against a RoShamBo algorithm. You may compile that as follows:

 gcc -o shell shell.c megahallc -lcurses

    * megahal.c (18Kb)
    * shell.c (15Kb)

 Prediction

 My opinion, as I have stated on the comp.ai.games newsgroup often enough,
 is that Darse's competition provides an ideal test-bed for predictive
 algorithms, or predictors. I have worked with predictors for the last five
 years, applying them to various syntactic pattern recognition problems,
 speech recognition, text generation and data compression.

 A predictor is an algorithm which is able to predict the next symbol in a
 sequence of symbols as a probability distribution over the alphabet of
 symbols. The only information available to the predictor is the history of
 symbols seen so far. In order to turn a predictor into a RoShamBo algorithm,
 we need to decide what the history of symbols should be, and how to turn a
 prediction into a RoShamBo move.

 Determining the history
      Because we are trying to predict our opponent's next move, and because
      our opponent may be using our previous moves to decide what they're
      going to do, it seems reasonable to make the symbol sequence an
      interlacing of both our histories: x1,y1,x2,y2,..., xn-1,yn-1, where x
      represents our opponent's move, y represents our move, and our job is
      to predict the value of xn in order to determine what yn should be.
 Choosing our move
      Once we have a prediction for yn in the form of a probability
      distribution over all possible moves, we need to decide what our move
      is going to be. We do this by choosing the move which maximises the
      expected value of our score. For example, imagine that the prediction
      gives P(rock)=0.45, P(paper)=0.15, P(scissors)=0.40. The maximum
      likelihood move (paper) gives an expected score of 1*0.45-1*0.40=0.05,
      while the defensive move of rock gives an expected score of
      1*0.40-1*0.15=0.25, and is therefore chosen.

 With these two modifications, any predictor may play RoShamBo!

 The MegaHAL Predictor

 MegaHAL is a very simple "infinite-order" Markov model. It stores frequency
 information about the moves the opponent has made in the past for all
 possible contexts (from a context of no symbols at all right up to a context
 of the entire history). In practise, the context is limited to forty-two
 symbols so that the algorithm satisfies the time constraint of playing one
 game every millisecond on average.

 MegaHAL stores this information in a ternary trie. Each context is
 represented as a node in this trie. Every time MegaHAL is asked to make a
 move, many of these nodes may activate, and each of them is capable of
 providing us with a prediction about what's coming next. We need to select
 one of them. We do this by storing in each node the average score that that
 node would have if it had been used exclusively in the past. We select the
 node which has the highest average score. If more than one node qualifies,
 we choose the one which takes the longest context into account.

 In some situations, no nodes will be selected. In this situation, we make a
 move at random.

 MegaHAL also gathers statistics over a sliding window, which means that it
 "forgets" about events which occurred a long time ago. This process allows
 MegaHAL to adapt more rapidly to changes in its opponents strategy. In the
 competition version, a sliding window of four hundred symbols was used (a
 match consists of two thousand symbols).

 Conclusion

 I hope that brief explanation of the MegaHAL strategy has been enlightening.
 I apologise for any omissions or poor English, and blame that on the fact
 that it was written at 12:45pm on a Saturday night, following a night out
 with friends!
*/

/*============================================================================*/
/*
 *  Copyright (C) 1999 Jason Hutchens
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the license or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the Gnu Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */
/*============================================================================*/
/*
 *      NB:      File displays best with tabstops set to three spaces.
 *
 *      Name:      MegaHAL (in honour of http://ciips.ee.uwa.edu.au/~hutch/hal/)
 *
 *      Author:   Jason Hutchens (hutch@amristar.com.au)
 *
 *      Purpose:   Play the game of Rock-Paper-Scissors.  Statistics about the
 *               game so far are recorded in a ternary trie, represnting an
 *               infinite-order Markov model.  The context which has performed
 *               best in the past is used to make the prediction, and we
 *               gradually fall-back through contexts which performed less well
 *               when the contexts haven't yet been observed.  One of the
 *               contexts is always guaranteed to make a move at random, so
 *               we never encounter a situation where we can't make a move.
 *               Statistics are gathered over a sliding window, allowing
 *               adaption if the opponent's strategies change.
 *
 *      $Id: megahal.c,v 1.8 1999/09/16 03:18:27 hutch Exp hutch $
 */
/*============================================================================*/
class HalBot : public RSBBot {
 public:
  /*
   *      These defines are the three heuristic parameters that can be modified
   *      to alter performance.  BELIEVE gives the number of times
   * a context must be observed before being used for prediction,
   * HISTORY gives the maximum context size to observe (we're
   * limited by time, not memory), and WINDOW gives the size of the
   * sliding window, 0 being infinite.
   *
   *      - BELIEVE>=1
   *      - HISTORY>=1
   *      - WINDOW>=HISTORY or 0 for infinite
   */
  static constexpr int BELIEVE = 1;
  static constexpr int HISTORY = 21;
  static constexpr int WINDOW = 200;

  HalBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int i, j;

    /*
     *      If this is the beginning of the game, set some things up.
     */
    if (history_len() == 0) {
      /*
       *      Clear the trie, by setting its size to unity, and clearing the
       *      statistics of the root node.
       */
      trie_.clear();
      trie_.push_back(NODE{0, {0, 0, 0}, {-1, -1, -1}});
      /*
       *      Clear the memory matrix.
       */
      for (i = 0; i < WINDOW; ++i)
        for (j = 0; j < HISTORY + 2; ++j) memory_[i][j] = -1;
      /*
       *      Clear the context array.
       */
      for (i = 0; i < HISTORY + 2; ++i) {
        context_[i] = CONTEXT{0, 0, 0, 0, 0};
      }
      context_[0] = CONTEXT{0, 0, 0, -1, -1};
      context_[1] = CONTEXT{0, 0, 0, 0, 0};

      // Note: fixed static variable which was not being reset.
      remember_ = 0;
    }

    /*
     *      If we've already started playing, evaluate how well we went on our
     *      last turn, and update our list which decides which contexts give the
     *      best predictions.
     */
    if (history_len() > 0) {
      /*
       *      We begin by forgetting which contexts performed well in the
       *      distant past.
       */
      if constexpr (WINDOW > 0) {
        for (i = 1; i < context_size_; ++i) {
          if (WINDOW - i > 0) {
            if (memory_[(remember_ + i - 1) % WINDOW][i] >= 0) {
              if (memory_[(remember_ + i - 1) % WINDOW][i] ==
                  ((opp_history()[history_len() - WINDOW + i - 1] + 1) % 3))
                context_[i].best -= 1;
              if (memory_[(remember_ + i - 1) % WINDOW][i] ==
                  ((opp_history()[history_len() - WINDOW + i - 1] + 2) % 3))
                context_[i].worst -= 1;
              context_[i].seen -= 1;
            }
          }
        }
      }

      /*
       *      Clear our dimmest memory.
       */
      if constexpr (WINDOW > 0)
        for (i = 0; i < context_size_; ++i) memory_[remember_][i] = -1;
      /*
       *      We then remember the contexts which performed the best most
       *      recently.
       */
      for (i = 0; i < context_size_; ++i) {
        if (context_[i].node >= trie_.size()) continue;
        if (context_[i].node == -1) continue;
        if (trie_[context_[i].node].total >= BELIEVE) {
          int expected[3];
          for (j = 0; j < 3; ++j)
            expected[j] = trie_[context_[i].node].move[(j + 2) % 3] -
                          trie_[context_[i].node].move[(j + 1) % 3];
          int last_move = 0;
          for (j = 1; j < 3; ++j)
            if (expected[j] > expected[last_move]) last_move = j;
          if (last_move == (opp_last_move() + 1) % 3) context_[i].best += 1;
          if (last_move == (opp_last_move() + 2) % 3) context_[i].worst += 1;
          context_[i].seen += 1;
          if constexpr (WINDOW > 0) {
            memory_[remember_][i] = last_move;
          }
        }
      }
      if constexpr (WINDOW > 0) {
        remember_ = (remember_ + 1) % WINDOW;
      }
    }

    /*
     *      Clear the context to the node which always predicts at random, and
     *      the root node.
     */
    context_size_ = 2;
    /*
     *      We begin by forgetting the statistics we've previously gathered
     *      about the symbol which is now leaving the sliding window.
     */
    if ((WINDOW > 0) && (history_len() - WINDOW > 0)) {
      for (i = history_len() - WINDOW;
           i < std::min(history_len() - WINDOW + HISTORY, history_len()); ++i) {
        /*
         *      Find the node which corresponds to the context.
         */
        int node = 0;
        for (j = std::max(history_len() - WINDOW, 1); j < i; ++j) {
          node = trie_[node].next[opp_history()[j]];
          node = trie_[node].next[my_history()[j]];
        }
        // if((node<0)||(node>=trie_.size()))fprintf(stderr, "OUCH\n");
        /*
         *      Update the statistics of this node with the opponents move.
         */
        trie_[node].total -= 1;
        trie_[node].move[opp_history()[i]] -= 1;
      }
    }

    /*
     *      Build up a context array pointing at all the nodes in the trie
     *      which predict what the opponent is going to do for the current
     *      history.  While doing this, update the statistics of the trie with
     *      the last move made by ourselves and our opponent.
     */
    const int loop_end = std::max(
        history_len() - (WINDOW > 0 ? std::min(WINDOW, HISTORY) : HISTORY), 0);
    for (i = history_len(); i > loop_end; --i) {
      /*
       *      Find the node which corresponds to the context.
       */
      int node = 0;
      for (j = i; j < history_len(); ++j) {
        node = trie_[node].next[opp_history()[j]];
        node = trie_[node].next[my_history()[j]];
      }
      // if((node<0)||(node>=trie_.size()))fprintf(stderr, "OUCH\n");
      /*
       *      Update the statistics of this node with the opponents move.
       */
      trie_[node].total += 1;
      trie_[node].move[opp_last_move()] += 1;
      /*
       *      Move to this node, making sure that we've allocated it first.
       */
      if (trie_[node].next[opp_last_move()] == -1) {
        trie_[node].next[opp_last_move()] = trie_.size();
        trie_.push_back(NODE{0, {0, 0, 0}, {-1, -1, -1}});
      }
      node = trie_[node].next[opp_last_move()];
      // if((node<0)||(node>=trie_.size()))fprintf(stderr, "OUCH\n");
      /*
       *      Move to this node, making sure that we've allocated it first.
       */
      if (trie_[node].next[my_last_move()] == -1) {
        trie_[node].next[my_last_move()] = trie_.size();
        trie_.push_back(NODE{0, {0, 0, 0}, {-1, -1, -1}});
      }
      node = trie_[node].next[my_last_move()];
      // if((node<0)||(node>=trie_.size()))fprintf(stderr, "OUCH\n");
      /*
       *      Add this node to the context array.
       */
      context_size_ += 1;
      context_[context_size_ - 1].node = node;
      context_[context_size_ - 1].size = context_size_ - 2;
    }
    /*
     *      Sort the context array, based upon what contexts have proved
     *      successful in the past.  We sort the context array by looking
     *      at the context lengths which most often give the best predictions.
     *      If two contexts give the same amount of best predictions, choose
     *      the longest one.  If two contexts have consistently failed in the
     *      past, choose the shortest one.
     */
    CONTEXT sorted[HISTORY + 2];
    for (i = 0; i < context_size_; ++i) sorted[i] = context_[i];
    qsort(sorted, context_size_, sizeof(CONTEXT), halbot_compare);
    /*
     *      Starting at the most likely context, gradually fall-back until we
     *      find a context which has been observed at least BELIEVE
     * times.  Use this context to predict a probability distribution over the
     * opponents possible moves.  Select the move which maximises the expected
     * score.
     */
    int move = -1;
    for (i = 0; i < context_size_; ++i) {
      if (sorted[i].node >= trie_.size()) continue;
      if (sorted[i].node == -1) break;
      if (trie_[sorted[i].node].total >= BELIEVE) {
        int expected[3];
        for (j = 0; j < 3; ++j)
          expected[j] = trie_[sorted[i].node].move[(j + 2) % 3] -
                        trie_[sorted[i].node].move[(j + 1) % 3];
        move = 0;
        for (j = 1; j < 3; ++j)
          if (expected[j] > expected[move]) move = j;
        break;
      }
    }
    /*
     *      If no prediction was possible, make a random move.
     */
    if (move == -1) move = random() % 3;
    /*
     *      Return our move.
     */
    return (move);
  }

 private:
  /*
   *      Each node of the trie contains frequency information about the moves
   *      made at the context represented by the node, and where the sequent
   *      nodes are in the array.
   */
  struct NODE {
    short int total;
    short int move[3];
    int next[3];
  };
  /*
   *      The context array contains information about contexts of various
   *      lengths, and this is used to select a context to make the prediction.
   */
  struct CONTEXT {
    int worst;
    int best;
    int seen;
    int size;
    int node;
  };

  /*----------------------------------------------------------------------------*/
  /*
   *      Function:   halbot_compare
   *
   *      Arguments:   const void *value1, const void *value2
   *                  Two pointers into the sort array.  Our job is to decide
   *                  whether value1 is less than, equal to or greater than
   *                  value2.
   *
   *      Returns:      An integer which is positive if value1<value2, negative
   * if value1>value2, and zero if they're equal.  Various heuristics are used
   * to test for this inequality.
   *
   *      Overview:   This comparison function is required by qsort().
   */
  static int halbot_compare(const void *value1, const void *value2) {
    /*
     *      Some local variables.
     */
    CONTEXT *context1;
    CONTEXT *context2;
    float prob1 = 0.0;
    float prob2 = 0.0;
    /*
     *      This looks complicated, but it's not.  Basically, given two nodes
     *      in the trie, these heuristics decide which node should be used to
     *      make a prediction first.  The rules are:
     *      - Choose the one which has performed the best in the past.
     *      - If they're both being tried for the first time, choose the
     * shortest.
     *      - If they've both performed equally well, choose the longest.
     */
    context1 = (CONTEXT *)value1;
    context2 = (CONTEXT *)value2;
    if (context1->seen > 0)
      prob1 =
          (float)(context1->best - context1->worst) / (float)(context1->seen);
    if (context2->seen > 0)
      prob2 =
          (float)(context2->best - context2->worst) / (float)(context2->seen);
    if (prob1 < prob2) return (1);
    if (prob1 > prob2) return (-1);
    // Note: bug of seen=0 left in place to duplicate competition bot.
    if ((context1->seen == 0) && (context2->seen = 0)) {
      if (context1->size < context2->size) return (-1);
      if (context1->size > context2->size) return (1);
      return (0);
    }
    if (context1->size < context2->size) return (1);
    if (context1->size > context2->size) return (-1);
    return (0);
  } /* end of "halbot_compare" */

  std::vector<NODE> trie_;
  int context_size_ = 0;
  CONTEXT context_[HISTORY + 2];
  int memory_[WINDOW][HISTORY + 2];
  int remember_ = 0;
};

}  // namespace roshambo_tournament

/*============================================================================*/
/*
 *      $Log: megahal.c,v $
 *      Revision 1.7  1999/09/16 03:16:55  hutch
 *      Did some speed improvements, improved the method of remembering past
 *      strategies, and imroved the heuristics for sorting.  Over 1000 tourneys
 *      of 1000 trials, it performed 17.6 times better than the second best bot,
 *      "Beat Last Move", and scored an average of 678 per match.  It also
 *      consistently beats version 1.1, scoring an average of 100 or so per
 *      match.
 *
 *      Revision 1.5  1999/09/13 16:51:57  hutch
 *      The sliding window is working perfectly.  Of course, this strategy
 *      doesn't improve the performance of MegaHAL-Infinite on the standard
 *      algorithms, but it will hopefully improve performance on smarter ones.
 *
 *      Revision 1.4  1999/09/13 14:48:57  hutch
 *      Cleaned up the source a bit, and prepared to implement the sliding
 *      window strategy.
 *
 *      Revision 1.3  1999/09/12 06:29:30  hutch
 *      Consideration of the statistics, and correcting it to give proper
 *      probability estimates, improved Megahal-Infinite beyond MegaHAL.
 *
 *      Revision 1.2  1999/09/12 06:23:02  hutch
 *      Infinite contexts are done, and we now choose the context that has
 *      performed the best in the past.  Doesn't perform as well as MegaHAL,
 *      but I believe it will perform better on craftier algorithms.  Plus
 *      it out-performs MegaHAL on R-P-S 20-20-60.
 *
 *      Revision 1.1  1999/09/12 03:53:08  hutch
 *      This is the first official version.  We are now going to concentrate
 *      on making an infinite-context model, and collecting statistics over
 *      a sliding window, in the hope that this will improve performance
 *      against more sophisticated algorithms.
 *
 *      Revision 0.4  1999/09/11 12:40:11  hutch
 *      Okay, experimentation with parameters has increased it's performance to
 *      about 15 times better than the second best bot, and it's near perfect on
 *      "Beat Last Move", "Beat Frequent Pick", "Rotate RPS" and "Good Ol Rock".
 *      It scores about half on "Always Switchin'", and about a third on "R-P-S
 *      20-20-60".  Interestingly, this is the only bot which it has difficulty
 *      with.  Over 1000 tourneys of 1000 trials, it performed 17.5 times better
 *      than the second best bot, "Beat Last Move", and scored an average of 677
 *      per match.
 *
 *      Revision 0.3  1999/09/11 12:33:54  hutch
 *      Everything is working; the program kicks ass against the standard bots
 *      (performing at least twelve times better than the second best).  I will
 *      fine-tune the algorithm a bit, although it is quite quick, and will play
 *      around with the heuristics before submitting.
 *
 *      Revision 0.2  1999/09/11 11:40:01  hutch
 *      The mechanism for selecting the best move has been finished, and the
 *      model is working for a NULL context.  Now we need to expand it to the
 *      infinite context.
 *
 *      Revision 0.1  1999/09/11 05:58:29  hutch
 *      Initial revision
 */
/*============================================================================*/

#endif  // HALBOT_H
