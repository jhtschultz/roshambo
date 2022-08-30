#ifndef ACTR_LAG2_DECAY_H
#define ACTR_LAG2_DECAY_H

#include <cmath>
#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  ACT-R Lag2 (13)   Dan Bothell, C Lebiere, R West (USA)

 RoShamBo player submission by Christian Lebiere, Robert West,
 and Dan Bothell

 This player is based on an ACT-R (http://act.psy.cmu.edu) model
 that plays RoShamBo.  The model can be played against on the web
 at http://act.psy.cmu.edu/ACT/ftp/models/RPS/index.html.

 suggested name "ACT-R Lag2"
 function name actr_lag2_decay
*/
/*
  C function that implements the math underlying the
  ACT-R (http://act.psy.cmu.edu) model of RPS by
  Christian Lebiere and Robert West (Published in the
  Proceedings of the Twenty-first Conference of the
  Cognitive Science Society.)
  This model stores in long-term memory sequences of
  moves and attempts to anticipate the opponent's
  moves by retrieving from memory the most active sequence.
  More information, and a web playable version avalable at:
  http://act.psy.cmu.edu/ACT/ftp/models/RPS/index.html
*/
class ActrLag2Decay : public RSBBot {
 public:
  ActrLag2Decay(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    double frequencies[3], score, p, best_score = kMaxRandom;

    int back1 = 0, back2 = 0, i, winner, index = history_len();

    winner = 0; /* -db */
    for (i = 0; i < 3; i++) frequencies[i] = pow(index + 1, -.5);

    if (index >= 2) {
      back2 = opp_history()[index - 1];
      back1 = opp_history()[index];
    }

    for (i = 0; i < index; i++) {
      if (i >= 2 && opp_history()[i - 1] == back2 && opp_history()[i] == back1)
        frequencies[opp_history()[i + 1]] += pow(index - i, -0.5);
    }

    for (i = 0; i < 3; i++) {
      /*
        Approximates a sample from a normal distribution with mean zero
        and s-value of .25 [s = sqrt(3 * sigma) / pi]
      */

      do {
        p = random();
      } while (p == 0.0);

      p /= kMaxRandom;
      p = .25 * log((1 - p) / p);

      /* end of noise calculation */
      score = p + log(frequencies[i]);

      if (best_score == kMaxRandom || score > best_score) {
        winner = i;
        best_score = score;
      }
    }
    return ((winner + 1) % 3);
  }
};

}  // namespace roshambo_tournament

#endif  // ACTR_LAG2_DECAY_H
