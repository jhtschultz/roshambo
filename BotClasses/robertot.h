#ifndef ROBERTOT_H
#define ROBERTOT_H

#include <algorithm>
#include <cstdlib>
#include <cstring>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Robertot (8)   Andreas Junghanns (Ger)  */
class Robertot : public RSBBot {
 public:
  static constexpr int NHIST = 10;
  static constexpr int NPREDICTS = 2;
  /* grains for the freq count % */
  static constexpr int STEPS = 200;
  /* maximal vote for 0/100% */
  static constexpr int MAXVOTE = 256;
  /* zero point for the distribution */
  static constexpr float ZERO = 11.1;

  Robertot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int p, h, pos, rsb, h_rsb, o_rsb;
    int vote[3];
    float index;

    if (history_len() == 0) {
      memset(hitsd, 0, sizeof(int) * NHIST * 3 * 3 * 3);
      memset(countd, 0, sizeof(int) * NHIST * 3 * 3);
      for (index = ((float)MAXVOTE) / FUNC(ZERO), p = 0, h = ZERO; h > 0; h--)
        incvote[p++] = -((int)((((float)FUNC(h)) * index) + 0.5));
      for (index = ((float)MAXVOTE) / FUNC(STEPS - ZERO), h = ZERO; h <= STEPS;
           h++)
        incvote[p++] = ((int)((((float)FUNC(h - ZERO)) * index) + 0.5));
    }
    if (history_len() >= NPREDICTS) {
      /* Only with enough data try to predict! */
      pos = history_len();
      rsb = opp_history()[pos];
      for (h = 0; h < NHIST && (pos - (h + 1)) > 0; h++) {
        countd[h][opp_history()[pos - (h + 1)]][my_history()[pos - (h + 1)]]++;
        hitsd[h][rsb][opp_history()[pos - (h + 1)]]
             [my_history()[pos - (h + 1)]]++;
      }
      for (rsb = 0; rsb < 3; rsb++) vote[rsb] = 0;
      /* Now, each history entry will vote for which move to play */
      for (rsb = 0; rsb < 3; rsb++) {
        for (h = 0; h < NHIST && (pos - h) > 0; h++) {
          o_rsb = opp_history()[pos - h];
          h_rsb = my_history()[pos - h];
          if (countd[h][o_rsb][h_rsb]) {
            index = ((float)STEPS) * hitsd[h][rsb][o_rsb][h_rsb] /
                    countd[h][o_rsb][h_rsb];
            vote[rsb] += incvote[(int)index];
          }
        }
      }
      h = std::min(vote[kRock], vote[kPaper]);
      h = std::min(h, vote[kScissors]);
      vote[kRock] -= h;
      vote[kPaper] -= h;
      vote[kScissors] -= h;
      h = std::max(vote[kRock], vote[kPaper]);
      h = std::max(h, vote[kScissors]);
      h = (h * 3) / 4;
      if (h == 0) h++;
      vote[kRock] /= h;
      vote[kPaper] /= h;
      vote[kScissors] /= h;
      if (vote[kRock] > vote[kScissors] && vote[kRock] > vote[kPaper])
        return (kPaper);
      else if (vote[kScissors] > vote[kPaper] && vote[kScissors] > vote[kRock])
        return (kRock);
      else if (vote[kPaper] > vote[kRock] && vote[kPaper] > vote[kScissors])
        return (kScissors);
      else if (vote[kRock] == vote[kPaper] && vote[kPaper] == vote[kScissors])
        return (random() % 3);
      else if (vote[kRock] == vote[kPaper])
        return (kPaper);
      else if (vote[kPaper] == vote[kScissors])
        return (kScissors);
      else if (vote[kScissors] == vote[kRock])
        return (kRock);
      else
        return (random() % 3); /* should never happen */
    } else {
      return (random() % 3);
    }
  }

 private:
  static float FUNC(float x) { return x * x * x * x * x; }

  /* gather stats for counts of related events, NHIST back */
  int hitsd[NHIST][3][3][3]; /* NHIST counts, for each combination */
  int countd[NHIST][3][3];   /* history was seen how many times */
  int incvote[STEPS + 1];
};

}  // namespace roshambo_tournament

#endif  // ROBERTOT_H
