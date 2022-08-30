#ifndef SWEETROCK_H
#define SWEETROCK_H

#include <algorithm>
#include <cstdlib>
#include <cstring>

#include "rsb_bot.h"

namespace roshambo_tournament {

/**********************************************************************/

/*  Entrant:  Sweet Rocky (24)   Lourdes Pena (Mex)  */

/*********** Lourdes Pena Castillo September, 1999 ***************/
/*********** Sweet Rocky program                    **************/
class SweetRock : public RSBBot {
 public:
  static constexpr int LMIN2 = 2;
  static constexpr int LBAD = -40;
  static constexpr float LTH = .80;

  SweetRock(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    /* play whatever will beat the opponent's most frequent choice after
       previous match history */
    int *pCount, *pLast, total, choice, pred;
    float diff;

    if (history_len() == 0) {
      memset(count, 0, sizeof(int) * 27);
      memset(lastTime, 0, sizeof(int) * 9);
      score = 0;
      goingbad = 0;
      return (biased_roshambo(0.33, 0.33)); /* Be optimal first */
    }

    if (history_len() < LMIN2) {
      if ((opp_last_move() - my_last_move() == 1) ||
          (opp_last_move() - my_last_move() == -2)) {
        score--;
      } else if (opp_last_move() != my_last_move()) {
        score++;
      }
      return (biased_roshambo(0.33, 0.33)); /* Be optimal first */
    }

    /* Add the previous result information */
    pCount = count[my_history()[history_len() - 1]]
                  [opp_history()[history_len() - 1]];
    pCount[opp_last_move()]++;

    if (opp_last_move() - my_last_move() == 1 ||
        opp_last_move() - my_last_move() == -2) {
      score--;
    } else if (opp_last_move() != my_last_move()) {
      score++;
    }

    if (score == LBAD) goingbad = 1;

    if (goingbad) {                           /* oh-oh! Things are going bad! */
      return (biased_roshambo(0.333, 0.333)); /* better be optimal then */
    }

    pLast = lastTime[my_history()[history_len() - 1]]
                    [opp_history()[history_len() - 1]];
    pLast[0] = opp_last_move();

    /* Look what the numbers say the opponent will do next */
    pCount = count[my_last_move()][opp_last_move()];
    total = pCount[kRock] + pCount[kPaper] + pCount[kScissors];

    if (total == 0) { /*Not information, then be optimal */
      return (biased_roshambo(0.33, 0.33));
    }

    /* What the opp. did last time */
    pLast = lastTime[my_last_move()][opp_last_move()];

    if ((pCount[kRock] > pCount[kPaper]) &&
        (pCount[kRock] > pCount[kScissors])) {
      pred = kRock;
      choice = kPaper;
    } else if (pCount[kPaper] > pCount[kScissors]) {
      pred = kPaper;
      choice = kScissors;
    } else {
      pred = kScissors;
      choice = kRock;
    }

    /* Maybe the choice is close! */
    if (pred != pLast[0]) {
      diff = (float)pCount[pLast[0]] / (float)pCount[pred];
      if (diff > LTH) {
        if (flip_biased_coin(1 - diff)) {
          return (pLast[0]);
        } else {
          return (choice);
        }
      }
    }
    return (choice);
  }

 private:
  int count[3][3][3];    /*[Idid][Itdid][Itdoes];*/
  int lastTime[3][3][1]; /*[Idid][Itdid][Itdid] */
  int score, goingbad;
};

}  // namespace roshambo_tournament

#endif  // SWEETROCK_H
