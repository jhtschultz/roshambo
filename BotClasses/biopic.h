#ifndef BIOPIC_H
#define BIOPIC_H

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Biopic (5)   Jonathan Schaeffer (Can)  */

/* RoShamBo -- Biopic version that switches between using opponent's and */
/* our history to decide on a strategy.                                  */
/*                                                                       */
/* Jonathan Schaeffer                                                    */
/* September 27, 1999    (debugged version, after the official event)    */
class Biopic : public RSBBot {
 public:
  static constexpr int WSIZE = 25;   /* Size of a losing margin? */
  static constexpr int CSIZE = 10;   /* Storage inefficient */
  static constexpr int EV_SCALE = 5; /* Used to determine a "small" value */
  static constexpr bool WEIGHTED =
      true; /* Bias towards more rather than less context */

  Biopic(int match_length) : RSBBot(match_length) {
    /* (1) First time the bot is run */
    int i, ix;
    for (i = 1, ix = 3; i < CSIZE; i++, ix *= 3) mult[i] = ix;
    mult[0] = 1;
    for (i = 0, ix = 3; i < CSIZE; i++, ix *= 3) {
      myh[i] = std::vector<int>(ix * 3);
      oph[i] = std::vector<int>(ix * 3);
    }
  }

  int GetAction() override {
    int i, j, wt[3];

    /*
     *
     * Initialize
     *
     */

    /* (2) First hand of a match */
    if (history_len() == 0) {
      score = gorandom = 0;
      for (i = 0; i < 4; ++i) {
        move[i] = 0;
        sc[i] = 0;
      }
      for (i = 0; i < 3; i++) freq[0][i] = freq[1][i] = 0;
      for (i = 0; i < CSIZE; i++) {
        std::fill(myh[i].begin(), myh[i].end(), 0);
        std::fill(oph[i].begin(), oph[i].end(), 0);
      }
    }

    /* (3) Last hand of the match */

    /* Statistics -- deleted */

    /* First hand - make a random move */
    if (history_len() <= 0) return (random() % 3);

    /*
     *
     * Process previous game
     *
     */

    /* (1) How is the match going? */
    if ((my_last_move() - opp_last_move() == 1) ||
        (my_last_move() - opp_last_move() == -2))
      score += 1;
    else if ((opp_last_move() - my_last_move() == 1) ||
             (opp_last_move() - my_last_move() == -2))
      score += -1;

    /* (2) Save context */
    freq[0][my_last_move()]++;
    freq[1][opp_last_move()]++;

    /* (3) How good are our predictions? */
    if (history_len() > 1) {
      for (i = 0; i < 4; i++)
        if (((move[i] + 2) % 3) == opp_last_move()) sc[i]++;
    }

    /* (4) Update context strings */
    for (j = opp_last_move(), i = 1; i <= CSIZE && history_len() - i > 0; i++) {
      j += opp_history()[history_len() - i] * 3 * mult[i - 1];
      oph[i - 1][j]++;
    }
    for (j = opp_last_move(), i = 1; i <= CSIZE && history_len() - i > 0; i++) {
      j += my_history()[history_len() - i] * 3 * mult[i - 1];
      myh[i - 1][j]++;
    }

    /* Periodically scale back results so that the program can */
    /* switch strategies.                      */
    if ((history_len() % 32) == 0) {
      for (i = 0; i < 3; i++) {
        freq[0][i] >>= 1;
        freq[1][i] >>= 1;
      }
      for (i = 0; i < 4; ++i) sc[i] >>= 1;
    }

    /*
     *
     * Use 4 special cases and 4  prediction models
     *
     *
     */

    /* (1) First move */
    /* Taken care of above */

    /* (2) If down too far, go random */
    if (score < -WSIZE) return (random() % 3);

    /* (3) Make a random move to confuse the opponent */
    if (gorandom) {
      if ((--gorandom) >= 8) return (random() % 3);
    }

    /* (4) If things not going well with our predictions, make */
    /* random moves for a while to confuse the opponent        */
    if (score <= -10 && gorandom == 0) {
      gorandom = 16;
      return (random() % 3);
    }

    /* (5) Use tables to predict next move using opponent info */
    /* Prediction 1                                            */
    BiopicWeight(wt, oph, opp_history());
    move[0] = BiopicMove(wt);

    /* (6) Use tables to predict next move using our info   */
    /* Prediction 2                                         */
    BiopicWeight(wt, myh, my_history());
    move[1] = BiopicMove(wt);

    /* (7) Check the frequency of the opponent's actions    */
    /* Prediction 3                                         */
    move[2] = BiopicMove(&freq[0][0]);

    /* (8) Check the frequency of the opponent's actions    */
    /* Prediction 4                                         */
    move[3] = BiopicMove(&freq[1][0]);

    /* Finally, we decide which strategy to use             */
    /* Use maximum sc for the move                          */
    for (j = 0, i = 1; i < 4; i++)
      if (sc[i] > sc[j]) j = i;
    /* Ta da */
    return (move[j]);
  }

 private:
  int BiopicMove(int* wt) {
    int ev[3], ttl, i;

    ev[kPaper] = wt[kRock] - wt[kScissors];
    ev[kRock] = wt[kScissors] - wt[kPaper];
    ev[kScissors] = wt[kPaper] - wt[kRock];

    /* Decide */

    /* Make small values 0 */
    ttl = ev[kRock] + ev[kPaper] + ev[kScissors];
    for (i = 0; i < 3; i++)
      if (ev[i] * EV_SCALE < ttl) ev[i] = 0;

    /* Make large values big */
    ttl = ev[kRock] + ev[kPaper] + ev[kScissors];
    for (i = 0; i < 3; i++)
      if (ev[i] * 5 / 3 >= ttl) ev[i] = 99999;

    /* Decide */
    ttl = ev[kRock] + ev[kPaper] + ev[kScissors];
    if (ttl <= 0)
      return (biased_roshambo((double)1.0 / 3, (double)1.0 / 3));
    else
      return (
          biased_roshambo((double)ev[kRock] / ttl, (double)ev[kPaper] / ttl));
  }

  void BiopicWeight(int wt[], const std::vector<int> context[],
                    const int* history) {
    int i, j, ptr[CSIZE];

    /* Get indices into context */
    for (j = i = 0; i < CSIZE && history_len() - i > 0; i++) {
      j += history[history_len() - i] * mult[i];
      ptr[i] = j * 3;
    }

    /* Process context */
    wt[kRock] = wt[kPaper] = wt[kScissors] = 0;
    for (i = 0; i < CSIZE && history_len() - i > 0; i++) {
      for (j = 0; j < 3; j++)
        wt[j] += context[i][ptr[i] + j] * (WEIGHTED ? mult[i] : 1);
    }
  }

  int score = 0;
  int gorandom, move[4], sc[4], freq[2][3];
  std::vector<int> myh[CSIZE];
  std::vector<int> oph[CSIZE];
  int mult[CSIZE];
};

}  // namespace roshambo_tournament

#endif  // BIOPIC_H
