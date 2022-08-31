#ifndef SUNCRAZYBOT_H
#define SUNCRAZYBOT_H

#include <algorithm>
#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* ------------------------------------------ Sunir's Crazybot */
/* If it ain't winnin', it might just punch itself in the head
** outta shear crazyness.
*/
class SunCrazybot : public RSBBot {
 public:
  SunCrazybot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    sunCRAZYBOT *pPlayer = &Player;

    /* Reset the player data if we're on a new player */
    if (history_len() == 0) {
      sunShuffleCrazybotPlayer(pPlayer);
      pPlayer->iLastTurn = 0;
    } else {
      int iResult = sunRoshamboComparison(my_last_move(), opp_last_move());

      if (iResult < 0)
        pPlayer->dShuffleProbability += 0.1;
      else if (iResult == 0)
        pPlayer->dShuffleProbability += 0.05;
    }

    if (flip_biased_coin(pPlayer->dShuffleProbability))
      sunShuffleCrazybotPlayer(pPlayer);

    return pPlayer->iLastTurn = pPlayer->aiTransform[pPlayer->iLastTurn];
  }

 private:
  struct sunCRAZYBOT {
    int iLastTurn = 0;
    int aiTransform[3];
    double dShuffleProbability;
  };

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

  /* Sets the transform table to a new random ordered set */
  static void sunShuffleCrazybotPlayer(sunCRAZYBOT *pPlayer) {
    for (int i = 0; i < 3; ++i)
      pPlayer->aiTransform[i] = biased_roshambo(1.0 / 3, 1.0 / 3);
    pPlayer->dShuffleProbability = 0.0;
  }

  sunCRAZYBOT Player;
};

}  // namespace roshambo_tournament

#endif  // SUNCRAZYBOT_H
