#ifndef IOCAINEBOT_H
#define IOCAINEBOT_H

#include <algorithm>
#include <array>
#include <cstdlib>
#include <vector>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Iocaine Powder (1)   Dan Egnor (USA)

    Winner of the First International RoShamBo Programming Competition

 Iocaine Powder             (from: http://ofb.net/~egnor/iocaine.html)

 They were both poisoned.

 Iocaine Powder is a heuristically designed compilation of strategies and
 meta-strategies, entered in Darse Billings' excellent First International
 RoShamBo Programming Competition. You may use its source code freely; I
 ask only that you give me credit for any derived works. I attempt here to
 explain how it works.

 Meta-Strategy

 RoShamBo strategies attempt to predict what the opponent will do. Given a
 successful prediction, it is easy to defeat the opponent (if you know they
 will play rock, you play paper). However, straightforward prediction will
 often fail; the opponent may not be vulnerable to prediction, or worse, they
 might have anticipated your predictive logic and played accordingly. Iocaine
 Powder's meta-strategy expands any predictive algorithm P into six possible
 strategies:

 P.0: naive application
      Assume the opponent is vulnerable to prediction by P; predict their
      next move, and play to beat it. If P predicts your opponent will play
      rock, play paper to cover rock. This is the obvious application of P.

 P.1: defeat second-guessing
      Assume the opponent thinks you will use P.0. If P predicts rock, P.0
      would play paper to cover rock, but the opponent could anticipate this
      move and play scissors to cut paper. Instead, you play rock to dull
      scissors.

 P.2: defeat triple-guessing
      Assume the opponent thinks you will use P.1. Your opponent thinks you
      will play rock to dull the scissors they would have played to cut the
      paper you would have played to cover the rock P would have predicted,
      so they will play paper to cover your rock. But you one-up them,
      playing scissors to cut their paper.

      At this point, you should be getting weary of the endless chain. "We
      could second-guess each other forever," you say. But no; because of the
      nature of RoShamBo, P.3 recommends you play paper -- just like P.0! So
      we're only left with these three strategies, each of which will suggest
      a different alternative. (This may not seem useful to you, but have
      patience.)

 P'.0: second-guess the opponent
      This strategy assumes the opponent uses P themselves against you.
      Modify P (if necessary) to exchange the position of you and your
      opponent. If P' predicts that you will play rock, you would expect
      your opponent to play paper, but instead you play scissors.

 P'.1, P'.2: variations on a theme
      As with P.1 and P.2, these represent "rotations" of the basic idea,
      designed to counteract your opponent's second-guessing.

 So, for even a single predictive algorithm P, we now have six possible
 strategies. One of them may be correct -- but that's little more useful
 than saying "one of rock, scissors, or paper will be the correct next
 move". We need a meta-strategy to decide between these six strategies.

 Iocaine Powder's basic meta-strategy is simple: Use past performance to
 judge future results.

 The basic assumption made by this meta-strategy is that an opponent will not
 quickly vary their strategy. Either they will play some predictive algorithm
 P, or they will play to defeat it, or use some level of second-guessing; but
 whatever they do, they will do it consistently. To take advantage of this
 (assumed) predictability, at every round Iocaine Powder measures how well
 each of the strategies under consideration (six for every predictive
 algorithm!)  would have done, if it had played them. It assigns each one a
 score, using the standard scoring scheme used by the tournament: +1 point if
 the strategy would have won the hand, -1 if it would have lost, 0 if it
 would have drawn.

 Then, to actually choose a move, Iocaine Powder simply picks the strategy
 variant with the highest score to date.

 The end result is that, for any given predictive technique, we will beat any
 contestant that would be beaten by the technique, any contestant using the
 technique directly, and any contestant designed to defeat the technique
 directly.

 Strategies

 All the meta-strategy in the world isn't useful without some predictive
 algorithms. Iocaine Powder uses three predictors:

 Random guess
      This "predictor" simply chooses a move at random. I include this
      algorithm as a hedge; if someone is actually able to predict and defeat
      Iocaine Powder with any regularity, before long the random predictor
      will be ranked with the highest score (since nobody can defeat
      random!). At that point, the meta-strategy will ensure that the program
      "cuts its losses" and starts playing randomly to avoid a devastating
      loss. (Thanks to Jesse Rosenstock for discovering the necessity of such
      a hedge.)

 Frequency analysis
      The frequency analyzer looks at the opponent's history, finds the move
      they have made most frequently in the past, and predicts that they will
      choose it. While this scores a resounding defeat again "Good Ole Rock",
      it isn't very useful against more sophisticated opponents (which are
      usually quite symmetrical). I include it mostly to defeat other
      competitors which use it as a predictive algorithm.

 History matching
      This is easily the strongest predictor in Iocaine Powder's arsenal, and
      variants of this technique are widely used in other strong entries. The
      version in Iocaine Powder looks for a sequence in the past matching the
      last few moves. For example, if in the last three moves, we played
      paper against rock, scissors against scissors, and scissors against
      rock, the predictor will look for times in the past when the same three
      moves occurred. (In fact, it looks for the longest match to recent
      history; a repeat of the last 30 moves is considered better than just
      the last 3 moves.) Once such a repeat is located, the history matcher
      examines the move our opponent made afterwards, and assumes they will
      make it again. (Thanks to Rudi Cilibrasi for introducing me to this
      algorithm, long ago.)

      Once history is established, this predictor easily defeats many weak
      contestants. Perhaps more importantly, the application of meta-strategy
      allows Iocaine to outguess other strong opponents.

 Details

 If you look at Iocaine Powder's source code, you'll discover additional
 features which I haven't treated in this simplified explanation. For
 example, the strategy arsenal includes several variations of frequency
 analysis and history matching, each of which looks at a different amount of
 history; in some cases, prediction using the last 5 moves is superior to
 prediction using the last 500. We run each algorithm with history sizes of
 1000, 100, 10, 5, 2, and 1, and use the general meta-strategy to figure out
 which one does best.

 In fact, Iocaine even varies the horizon of its meta-strategy analyzer!
 Strategies are compared over the last 1000, 100, 10, 5, 2, and 1 moves, and
 a meta-meta-strategy decides which meta-strategy to use (based on which
 picker performed best over the total history of the game). This was designed
 to defeat contestants which switch strategy, and to outfox variants of the
 simpler, older version of Iocaine Powder.

 Summary

 One must remember, when participating in a contest of this type, that we are
 not attempting to model natural phenomena or predict user actions; instead,
 these programs are competing against hostile opponents who are trying very
 hard not to be predictable. Iocaine Powder doesn't use advanced statistical
 techniques or Markov models (though it could perhaps be improved if it did),
 but it is very devious.

 It is, however, by no means the last word. Iocaine may have defeated all
 comers in the first tournament, but I have no doubt that my opponents will
 rise to the challenge in upcoming events.
   ------------------------------------------------------------------------
   Dan Egnor
*/
class IocaineBot : public RSBBot {
 public:
  static constexpr int my_hist = 0;
  static constexpr int opp_hist = 1;
  static constexpr int both_hist = 2;
  static constexpr int num_ages = 6;
  static constexpr std::array<int, num_ages> ages = {1000, 100, 10, 5, 2, 1};
  static constexpr std::array<int, 3> will_beat = {1, 2, 0};
  static constexpr std::array<int, 3> will_lose_to = {2, 0, 1};

  IocaineBot(int match_length) : RSBBot(match_length), p_(match_length) {}

  int GetAction() override { return iocaine(&p_); }

 private:
  struct Stats {
    Stats(int match_length) : sum(1 + match_length) {}
    std::vector<std::array<int, 3>> sum;
    int age = 0;
  };

  struct Predict {
    Predict(int match_length) : st(match_length) {}
    Stats st;
    int last = 0;
  };

  struct Iocaine {
    Iocaine(int match_length)
        : pr_history(num_ages,
                     std::array<std::array<Predict, 2>, 3>(
                         {std::array<Predict, 2>(
                              {Predict(match_length), Predict(match_length)}),
                          std::array<Predict, 2>(
                              {Predict(match_length), Predict(match_length)}),
                          std::array<Predict, 2>({Predict(match_length),
                                                  Predict(match_length)})})),
          pr_freq(num_ages, std::array<Predict, 2>({Predict(match_length),
                                                    Predict(match_length)})),
          pr_fixed(match_length),
          pr_random(match_length),
          pr_meta(num_ages, Predict(match_length)),
          stats({Stats(match_length), Stats(match_length)}) {}
    std::vector<std::array<std::array<Predict, 2>, 3>> pr_history;
    std::vector<std::array<Predict, 2>> pr_freq;
    Predict pr_fixed;
    Predict pr_random;
    std::vector<Predict> pr_meta;
    std::array<Stats, 2> stats;
  };

  int match_single(int i, int num, const int *history) {
    const int *highptr = history + num;
    const int *lowptr = history + i;
    while (lowptr > history && *lowptr == *highptr) --lowptr, --highptr;
    return history + num - highptr;
  }

  int match_both(int i, int num) {
    int j;
    for (j = 0; j < i && opp_history()[num - j] == opp_history()[i - j] &&
                my_history()[num - j] == my_history()[i - j];
         ++j)
      ;
    return j;
  }

  void do_history(int age, int best[3]) {
    const int num = history_len();
    int best_length[3], i, j, w;

    for (w = 0; w < 3; ++w) best[w] = best_length[w] = 0;
    for (i = num - 1; i > num - age && i > best_length[my_hist]; --i) {
      j = match_single(i, num, my_history());
      if (j > best_length[my_hist]) {
        best_length[my_hist] = j;
        best[my_hist] = i;
        if (j > num / 2) break;
      }
    }

    for (i = num - 1; i > num - age && i > best_length[opp_hist]; --i) {
      j = match_single(i, num, opp_history());
      if (j > best_length[opp_hist]) {
        best_length[opp_hist] = j;
        best[opp_hist] = i;
        if (j > num / 2) break;
      }
    }

    for (i = num - 1; i > num - age && i > best_length[both_hist]; --i) {
      j = match_both(i, num);
      if (j > best_length[both_hist]) {
        best_length[both_hist] = j;
        best[both_hist] = i;
        if (j > num / 2) break;
      }
    }
  }

  void reset_stats(Stats *st) {
    int i;
    st->age = 0;
    for (i = 0; i < 3; ++i) st->sum[st->age][i] = 0;
  }

  void add_stats(Stats *st, int i, int delta) { st->sum[st->age][i] += delta; }

  void next_stats(Stats *st) {
    if (st->age < rsb_trials()) {
      int i;
      ++(st->age);
      for (i = 0; i < 3; ++i) st->sum[st->age][i] = st->sum[st->age - 1][i];
    }
  }

  int max_stats(const Stats *st, int age, int *which, int *score) {
    int i;
    *which = -1;
    for (i = 0; i < 3; ++i) {
      int diff;
      if (age > st->age)
        diff = st->sum[st->age][i];
      else
        diff = st->sum[st->age][i] - st->sum[st->age - age][i];
      if (diff > *score) {
        *score = diff;
        *which = i;
      }
    }

    return -1 != *which;
  }

  void reset_predict(Predict *pred) {
    reset_stats(&pred->st);
    pred->last = -1;
  }

  /* last: opponent's last move (-1 if none)
     | guess: algorithm's prediction of opponent's next move */
  void do_predict(Predict *pred, int last, int guess) {
    if (-1 != last) {
      const int diff = (3 + last - pred->last) % 3;
      add_stats(&pred->st, will_beat[diff], 1);
      add_stats(&pred->st, will_lose_to[diff], -1);
      next_stats(&pred->st);
    }

    pred->last = guess;
  }

  void scan_predict(Predict *pred, int age, int *move, int *score) {
    int i;
    if (max_stats(&pred->st, age, &i, score)) *move = ((pred->last + i) % 3);
  }

  int iocaine(Iocaine *i) {
    const int num = history_len();
    const int last = (num > 0) ? opp_history()[num] : -1;
    const int guess = biased_roshambo(1.0 / 3.0, 1.0 / 3.0);
    int w, a, p;

    if (0 == num) {
      for (a = 0; a < num_ages; ++a) {
        reset_predict(&i->pr_meta[a]);
        for (p = 0; p < 2; ++p) {
          for (w = 0; w < 3; ++w) reset_predict(&i->pr_history[a][w][p]);
          reset_predict(&i->pr_freq[a][p]);
        }
      }
      for (p = 0; p < 2; ++p) reset_stats(&i->stats[p]);
      reset_predict(&i->pr_random);
      reset_predict(&i->pr_fixed);
    } else {
      add_stats(&i->stats[0], my_history()[num], 1);
      add_stats(&i->stats[1], opp_history()[num], 1);
    }

    for (a = 0; a < num_ages; ++a) {
      int best[3];
      do_history(ages[a], best);
      for (w = 0; w < 3; ++w) {
        const int b = best[w];
        if (0 == b) {
          do_predict(&i->pr_history[a][w][0], last, guess);
          do_predict(&i->pr_history[a][w][1], last, guess);
          continue;
        }
        do_predict(&i->pr_history[a][w][0], last, my_history()[b + 1]);
        do_predict(&i->pr_history[a][w][1], last, opp_history()[b + 1]);
      }

      for (p = 0; p < 2; ++p) {
        int best = -1, freq;
        if (max_stats(&i->stats[p], ages[a], &freq, &best))
          do_predict(&i->pr_freq[a][p], last, freq);
        else
          do_predict(&i->pr_freq[a][p], last, guess);
      }
    }

    do_predict(&i->pr_random, last, guess);
    do_predict(&i->pr_fixed, last, 0);

    for (a = 0; a < num_ages; ++a) {
      int aa, score = -1, move = -1;
      for (aa = 0; aa < num_ages; ++aa) {
        for (p = 0; p < 2; ++p) {
          for (w = 0; w < 3; ++w)
            scan_predict(&i->pr_history[aa][w][p], ages[a], &move, &score);
          scan_predict(&i->pr_freq[aa][p], ages[a], &move, &score);
        }
      }

      scan_predict(&i->pr_random, ages[a], &move, &score);
      scan_predict(&i->pr_fixed, ages[a], &move, &score);
      do_predict(&i->pr_meta[a], last, move);
    }

    {
      int score = -1, move = -1;
      for (a = 0; a < num_ages; ++a)
        scan_predict(&i->pr_meta[a], rsb_trials(), &move, &score);
      return move;
    }
  }

  Iocaine p_;
};

}  // namespace roshambo_tournament

#endif  // IOCAINEBOT_H
