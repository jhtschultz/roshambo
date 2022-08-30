#ifndef PIEDRA_H
#define PIEDRA_H

#include <cstdlib>
#include <cstring>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Piedra (25)   Lourdes Pena (Mex)  */

/*********** Lourdes Pena Castillo September, 1999 ***************/
/*********** Piedra  program                        **************/
class Piedra : public RSBBot {
 public:
  static constexpr int LMIN1 = 2;
  static constexpr int LBAD = -40;

  Piedra(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    /* play whatever will beat the opponent's most frequent choice
       after previous match history */

    int *pCount, total;

    if (history_len() == 0) {
      memset(Count, 0, sizeof(int) * 27);
      score = 0;
      goingbad = 0;
      return (biased_roshambo(0.33, 0.33)); /* Be optimal first */
    }

    if (history_len() < LMIN1) {
      if ((opp_last_move() - my_last_move() == 1) ||
          (opp_last_move() - my_last_move() == -2)) {
        score--;
      } else if (opp_last_move() != my_last_move()) {
        score++;
      }
      return (biased_roshambo(0.33, 0.33)); /* Be optimal first */
    }

    /* Add the previous result information */
    pCount = Count[my_history()[history_len() - 1]]
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

    /* Look what the numbers say the opponent will do next */
    pCount = Count[my_last_move()][opp_last_move()];
    total = pCount[kRock] + pCount[kPaper] + pCount[kScissors];

    if (total == 0) { /*Not information, then be optimal */
      return (biased_roshambo(0.33, 0.33));
    }

    if ((pCount[kRock] > pCount[kPaper]) &&
        (pCount[kRock] > pCount[kScissors])) {
      return (kPaper);
    } else if (pCount[kPaper] > pCount[kScissors]) {
      return (kScissors);
    } else {
      return (kRock);
    }
  }

 private:
  int Count[3][3][3]; /*[Idid][Itdid][Itdoes];*/
  int score, goingbad;
};

}  // namespace roshambo_tournament

#endif  // PIEDRA_H
