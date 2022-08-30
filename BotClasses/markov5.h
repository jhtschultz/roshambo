#ifndef MARKOV5_H
#define MARKOV5_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Majikthise (15)   Markian Hlynka (Can)  */
/*  Entrant:  Vroomfondel (18)   Markian Hlynka (Can)  */
// Use bails=false for markov5/Majikthise, true for markovbails/Vroomfondel.
class Markov5 : public RSBBot {
 public:
  static constexpr int markovLength = 243;
  static constexpr int windowSize = 5;

  Markov5(int match_length, bool bails = false)
      : RSBBot(match_length), bails_(bails) {}

  int GetAction() override {
    /* This bot is designed to win the current match. */
    int i, j;
    int markovindex;
    int nonzeros;
    int score;
    double newprob, cumprob;
    int retval; /* the value to return */
    double percentWins, percentLosses, margin;

    retval = 0;             /* -db */
    if (history_len() == 0) /* if we're just starting, init the array. */
    {
      for (i = 0; i < markovLength; i++) /* for every row */
      {
        Markovuse_[i] = 0;
        for (j = 0; j < 3; j++) /* reset every column */
        {
          MarkovChain_[i][j] = 0.33;
          MarkovTally_[i][j] = 0;
        }
      }
      /* now set our watch vars and stats accumulators */
      wins_ = losses_ = 0;
      percentWins = percentLosses = 0;

    } else {
      /* check if we won or lost on the last turn */
      if ((opp_last_move() + 1) % 3 == my_last_move())
        wins_++;
      else if ((opp_last_move() + 2) % 3 == my_last_move())
        losses_++;

      /* accumulate our stats       */
      percentWins = (double)wins_ / (double)history_len();
      percentLosses = (double)losses_ / (double)history_len();
    } /* else */

    /*******This is where we update the markov chain**************/

    /* regardless, update the markov chain. */

    if (history_len() > windowSize) /* if we're past P1.. remember, P1=P0 */
    {
      markovindex = 0;
      for (i = windowSize; i >= 1; i--)
        markovindex += ((i == 1) ? (opp_history()[history_len() - i])
                                 : (opp_history()[history_len() - i] * 3));

      /* now if we haven't used the row before, zero it and put a one in the */
      /* right place */
      if (!Markovuse_[markovindex]) {
        Markovuse_[markovindex] = 1;
        for (j = 0; j < 3; j++) MarkovChain_[markovindex][j] = 0;
        MarkovChain_[markovindex][opp_last_move()] = 1.0;
        MarkovTally_[markovindex][opp_last_move()]++;
      }    /* if */
      else /* ah. it's been used before, so now we distribut it across all used
              ones. */
      {    /* duh. don't forget to check that this is a new one */
        MarkovTally_[markovindex][opp_last_move()]++;

        nonzeros = 0;

        /* count how many have been used (are non-zero). */
        for (j = 0; j < 3; j++) nonzeros += MarkovTally_[markovindex][j];
        /* add one */
        newprob = 1.0 / ((double)nonzeros);

        /* distribute that value among them. */
        for (j = 0; j < 3; j++)
          if (MarkovTally_[markovindex][j] > 0)
            MarkovChain_[markovindex][j] =
                newprob * (double)MarkovTally_[markovindex][j];
      } /* else */
    }

    /**********************/

    margin = percentWins - percentLosses;
    score = wins_ - losses_;

    /* if we're more that 60% behind or ahead, bail. also, if we haven't done */
    /* even one move, don't use the markov chain if we don't have a previous */
    /* move to look up. */
    if ((history_len() <= windowSize) || (bails_ && (score < -50)))
      retval = (biased_roshambo(0.33333, 0.33333));

    else {
      /* if we didn't bail, we use the markov chain */
      /* for now use random */
      /* retval=(biased_roshambo(0.33333,0.33333)); */

      markovindex = 0;
      for (i = windowSize - 1; i >= 0; i--)
        markovindex += ((i == 0) ? (opp_history()[history_len() - i])
                                 : (opp_history()[history_len() - i] * 3));

      /* generate a continuous uniform variate */
      newprob = random() / kMaxRandom;
      /* now do a cumulative probability. */
      cumprob = 0;
      for (j = 0; j < 3; j++) {
        cumprob += MarkovChain_[markovindex][j];
        if (newprob < cumprob) {
          retval = (j + 1) % 3;
          break;
        }
      } /* for */
      if (!(newprob <
            cumprob)) /* test to make sure we don't have floating point error */
        retval = 0;   /*((2+1)%3)*/
    }

    return (retval);
  }

 private:
  bool bails_ = false;
  int wins_, losses_;

  double MarkovChain_[markovLength][3];
  int Markovuse_[markovLength];
  int MarkovTally_[markovLength][3];
};

}  // namespace roshambo_tournament

#endif  // MARKOV5_H
