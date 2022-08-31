#ifndef SUNNERVEBOT_H
#define SUNNERVEBOT_H

#include <algorithm>
#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

namespace {
/* These are magic numbers */
constexpr double sunNerveAttenuateLoss(double dValue) {
  /* Pulls value towards 0.0 */
  return dValue * 0.8;
}

constexpr double sunNerveAttenuateTie(double dValue) {
  /* Pulls value towards 0.0 */
  return dValue * 0.9;
}

constexpr double sunNerveAttenuateWin(double dValue) {
  /* Pulls value towards 1.0 but never exceeds 1.0 */
  return (dValue - 1.0) * 0.8 + 1.0;
}
}  // namespace

/* ------------------------------------------ Sunir's Nervebot */
/* Uses a nervous network whose input vector is
** (my last turn, opponents last turn)
*/
class SunNervebot : public RSBBot {
 public:
  SunNervebot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    /* Attenuate from last turn */
    int iResult = sunRoshamboComparison(my_last_move(), opp_last_move());

    static constexpr double (*apfnAttenuations[])(double dValue) = {
        sunNerveAttenuateLoss,
        sunNerveAttenuateTie,
        sunNerveAttenuateWin,
    };

    double dDelta;
    int iNextProbability, iOtherProbability;
    double dNextProbability, dOtherProbability;

    sunNERVEBOT *pPlayer = &Player;

    /* Reset the player data if we're on a new player */
    if (!history_len()) pPlayer->bInitialized = 0;

    if (!pPlayer->bInitialized) sunInitializeNervebotPlayer(pPlayer);

    /* First turn */
    if (!history_len()) return biased_roshambo(1.0 / 3, 1.0 / 3);

    /* Reward/punish based on last turn's vector and result */
    dDelta = pPlayer->adProbabilities[sunMYPREVTURN(2)][sunOPPPREVTURN(2)]
                                     [my_last_move()];

    pPlayer
        ->adProbabilities[sunMYPREVTURN(2)][sunOPPPREVTURN(2)][my_last_move()] =
        apfnAttenuations[iResult + 1](dDelta);

    dDelta -= pPlayer->adProbabilities[sunMYPREVTURN(2)][sunOPPPREVTURN(2)]
                                      [my_last_move()];

    /* Propogate the delta throughout the remaining probabilities */
    iNextProbability = (my_last_move() + 1) % 3;
    iOtherProbability = (iNextProbability + 1) % 3;

    dNextProbability =
        pPlayer->adProbabilities[sunMYPREVTURN(2)][sunOPPPREVTURN(2)]
                                [iNextProbability];
    dOtherProbability =
        pPlayer->adProbabilities[sunMYPREVTURN(2)][sunOPPPREVTURN(2)]
                                [iOtherProbability];

    /* Distributes the delta weighted to the magnitude of the
    ** two other choices' respective probabilities
    */
    dDelta = dDelta * dNextProbability / (dNextProbability + dOtherProbability);

    pPlayer->adProbabilities[sunMYPREVTURN(2)][sunOPPPREVTURN(2)]
                            [iNextProbability] += dDelta;

    pPlayer->adProbabilities[sunMYPREVTURN(2)][sunOPPPREVTURN(2)]
                            [iOtherProbability] =
        1.0 -
        pPlayer->adProbabilities[sunMYPREVTURN(2)][sunOPPPREVTURN(2)]
                                [iNextProbability] -
        pPlayer->adProbabilities[sunMYPREVTURN(2)][sunOPPPREVTURN(2)]
                                [my_last_move()];

    /* React to new vector */
    return biased_roshambo(
        pPlayer->adProbabilities[my_last_move()][opp_last_move()][kRock],
        pPlayer->adProbabilities[my_last_move()][opp_last_move()][kPaper]);
  }

 private:
  struct sunNERVEBOT {
    int bInitialized;

    /* [ways of arranging my last turn]
    ** [ways of arranging opponent's last turn]
    ** [ways of arranging my next turn]
    */
    double adProbabilities[3][3][3];
  };

  /* Sets the player's matrix to initially random probabilities,
  ** taking care to ensure the probabilities sum to 1.0 for each
  ** input vector.
  */
  void sunInitializeNervebotPlayer(sunNERVEBOT *pPlayer) {
    int i, j;

    pPlayer->bInitialized = 1;

    for (i = 3; i--;)
      for (j = 3; j--;) {
        pPlayer->adProbabilities[i][j][0] =
            (double)random() / (double)kMaxRandom;

        pPlayer->adProbabilities[i][j][1] =
            ((double)random() / (double)kMaxRandom) *
            (1.0 - pPlayer->adProbabilities[i][j][0]);

        pPlayer->adProbabilities[i][j][2] = 1.0 -
                                            pPlayer->adProbabilities[i][j][0] -
                                            pPlayer->adProbabilities[i][j][1];
      }
  }

  /* Returns -1 on a loss, 0 on a tie, 1 on a win */
  int sunRoshamboComparison(int me, int opp) {
    static constexpr int aiCompareTable[] = {
        kPaper,    /* rock */
        kScissors, /* paper */
        kRock,     /* scissors */
    };

    if (me == opp) return 0;

    return (aiCompareTable[me] == opp) ? -1 : 1;
  }

  int sunMYPREVTURN(int x) { return my_history()[history_len() - x + 1]; }
  int sunOPPPREVTURN(int x) { return opp_history()[history_len() - x + 1]; }

  sunNERVEBOT Player;
};

}  // namespace roshambo_tournament

#endif  // SUNNERVEBOT_H
