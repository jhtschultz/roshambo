#ifndef MARBLE_H
#define MARBLE_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Marble (19)   Aaron Davidson (Can)  */
/**************************************************************
 * MARBLE  1.4 By Aaron Davidson, Sept. 1999                  *
 * davidson@cs.ualberta.ca                                    *
 **************************************************************/
class Marble : public RSBBot {
 public:
  Marble(int match_length) : RSBBot(match_length) {}
  static constexpr int DEPTH = 3;
  static constexpr int NUM_RECENT = 20;

  int GetAction() override {
    int i, j, k, m, r, p, s;
    float noise, pR, pP, pS;

    /* number of games played */
    int ng = history_len();
    int oL = opp_history()[ng];
    int mL = my_history()[ng];

    /********************************************/

    /* FIRST MOVE -- INIT ARRAYS */
    if (ng == 0) {
      wins = 0;
      for (i = 0; i < 3; i++) {
        p_opp_0[i] = 0;
        p_my_0[i] = 0;
        for (j = 0; j < 3; j++) {
          p_opp_1[i][j] = 0;
          p_my_1[i][j] = 0;
          for (k = 0; k < 3; k++) {
            p_opp_2[i][j][k] = 0;
            p_my_2[i][j][k] = 0;
            p_oppmy_2[i][j][k] = 0;
            p_myopp_2[i][j][k] = 0;
            for (m = 0; m < 3; m++) {
              p_opp_3[i][j][k][m] = 0;
              p_my_3[i][j][k][m] = 0;
            }
          }
        }
      }
      return (RAND_INT(3));
    }

    if (last_pred == oL) wins++;

    noise = (float)(wins / ng);

    /* TABULATE OVERALL FEQUENCY OF ACTIONS */
    p_opp_0[oL]++;
    p_my_0[mL]++;

    r = p_opp_0[kRock];
    p = p_opp_0[kPaper];
    s = p_opp_0[kScissors];

    /* GET FREQUENCIES OF ACTIONS FOLLOWING OUR LAST MOVES */
    if (ng > 1) { /* DEPTH == 1 */
      int oL1 = opp_history()[ng - 1];
      int mL1 = my_history()[ng - 1];

      ++p_opp_1[oL][oL1];
      ++p_my_1[oL][mL1];
      r += p_opp_1[kRock][oL] + p_my_1[kRock][mL];
      p += p_opp_1[kPaper][oL] + p_my_1[kPaper][mL];
      s += p_opp_1[kScissors][oL] + p_my_1[kScissors][mL];

      if (ng > 2 && DEPTH >= 2) { /* DEPTH == 2 */
        int oL2 = opp_history()[ng - 2];
        int mL2 = my_history()[ng - 2];

        ++p_opp_2[oL][oL1][oL2];
        ++p_my_2[oL][mL1][mL2];
        ++p_oppmy_2[oL][oL1][mL2];
        ++p_myopp_2[oL][mL1][oL2];

        r += p_opp_2[kRock][oL][oL1] + p_my_2[kRock][mL][mL1] +
             p_oppmy_2[kRock][oL][mL1] + p_myopp_2[kRock][mL][oL1];
        p += p_opp_2[kPaper][oL][oL1] + p_my_2[kPaper][mL][mL1] +
             p_oppmy_2[kPaper][oL][mL1] + p_myopp_2[kPaper][mL][oL1];
        s += p_opp_2[kScissors][oL][oL1] + p_my_2[kScissors][mL][mL1] +
             p_oppmy_2[kScissors][oL][mL1] + p_myopp_2[kScissors][mL][oL1];

        if (ng > 3 && DEPTH >= 3) { /* DEPTH == 3 */
          int oL3 = opp_history()[ng - 3];
          int mL3 = my_history()[ng - 3];

          ++p_opp_3[oL][oL1][oL2][oL3];
          ++p_my_3[oL][mL1][mL2][mL3];

          r += p_opp_3[kRock][oL][oL1][oL2] + p_my_3[kRock][mL][mL1][mL2];
          p += p_opp_3[kPaper][oL][oL1][oL2] + p_my_3[kPaper][mL][mL1][mL2];
          s += p_opp_3[kScissors][oL][oL1][oL2] +
               p_my_3[kScissors][mL][mL1][mL2];
        }
      }
    }

    pR = r / (float)(r + p + s);
    pP = p / (float)(r + p + s);
    pS = s / (float)(r + p + s);

    if (pR > pP && pR > pS) { /* predict rock */
      last_pred = kRock;
      if (flip_biased_coin(noise * pS))
        return kRock;
      else if (flip_biased_coin(noise * pP))
        return kScissors;
      else
        return kPaper;
    } else if (pP > pR && pP > pS) { /* predict paper */
      last_pred = kPaper;
      if (flip_biased_coin(noise * pS))
        return kRock;
      else if (flip_biased_coin(noise * pR))
        return kPaper;
      else
        return kScissors;
    } else if (pS > pP && pS > pR) { /* predict scissors */
      last_pred = kScissors;
      if (flip_biased_coin(noise * pR))
        return kPaper;
      else if (flip_biased_coin(noise * pP))
        return kScissors;
      else
        return kRock;
    } else if (pR == pS && pR == pP) {
      last_pred = -1;
      return (RAND_INT(3));
    } else if (pR == pP) {
      last_pred = biased_roshambo(0.5, 0.5);
      if (flip_biased_coin(noise * pS)) return kRock;
      return last_pred;
    } else if (pR == pS) {
      last_pred = biased_roshambo(0.5, 0.0);
      if (flip_biased_coin(noise * pP)) return kScissors;
      return last_pred;
    } else if (pS == pP) {
      last_pred = biased_roshambo(0.0, 0.5);
      if (flip_biased_coin(noise * pR)) return kPaper;
      return last_pred;
    }
    last_pred = -1;
    return (RAND_INT(3));
  }

 private:
  static int RAND_INT(int x) { return (int)(random() / kMaxRandom * x); }

  /* persistent tallies of contextual frequencies */
  int p_opp_0[3];
  int p_my_0[3];
  int p_opp_1[3][3];
  int p_my_1[3][3];
  int p_opp_2[3][3][3];
  int p_my_2[3][3][3];
  int p_oppmy_2[3][3][3];
  int p_myopp_2[3][3][3];
  int p_opp_3[3][3][3][3];
  int p_my_3[3][3][3][3];

  int last_pred = -1;
  int wins;
};

}  // namespace roshambo_tournament

#endif  // MARBLE_H
