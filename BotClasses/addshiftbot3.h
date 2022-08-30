#ifndef ADDSHIFTBOT3_H
#define ADDSHIFTBOT3_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* base on sum of previous pair (my & opp), shift if losing */
/* deterministic 80% of the time, thus max -EV = -0.800 ppt */
class AddshiftBot3 : public RSBBot {
 public:
  AddshiftBot3(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int mv, diff;

    mv = history_len();
    if (mv == 0) {
      gear_ = 0;
      recent_ = 0;
      score_ = 0;
      return (random() % 3);
    }

    diff = (my_history()[mv] - opp_history()[mv] + 3) % 3;
    if (diff == 1) {
      score_++;
    }
    if (diff == 2) {
      score_--;
    }
    recent_++;

    if (((recent_ <= 20) && (score_ <= -3)) ||
        ((recent_ > 20) && (score_ <= -recent_ / 10))) {
      /* printf("switching gears at turn %d (%d / %d)\n", mv, score, recent); */
      gear_ += 2;
      recent_ = 0;
      score_ = 0;
    }
    if (flip_biased_coin(0.2)) {
      return (random() % 3);
    } else {
      return ((my_history()[mv] + opp_history()[mv] + gear_) % 3);
    }
  }

 private:
  int gear_;
  int recent_;
  int score_;
};

}  // namespace roshambo_tournament

#endif  // ADDSHIFTBOT_H
