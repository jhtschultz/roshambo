#ifndef ADDDRIFTBOT2_H
#define ADDDRIFTBOT2_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* base on sum of previous pair (my & opp), drift over time */
/* deterministic 50% of the time, thus max -EV = -0.500 ppt */
class AdddriftBot2 : public RSBBot {
 public:
  AdddriftBot2(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int mv;

    mv = history_len();
    if (mv == 0) {
      gear_ = 0;
      return (random() % 3);
    } else if (mv % 200 == 0) {
      gear_ += 2;
    }

    if (flip_biased_coin(0.5)) {
      return (random() % 3);
    } else {
      return ((my_history()[mv] + opp_history()[mv] + gear_) % 3);
    }
  }

 private:
  int gear_;
};

}  // namespace roshambo_tournament

#endif  // ADDDRIFTBOT_H
