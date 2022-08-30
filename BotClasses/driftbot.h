#ifndef DRIFTBOT_H
#define DRIFTBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* bias decision by opponent's last move, but drift over time */
class DriftBot : public RSBBot {
 public:
  DriftBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int mv, choice;

    mv = history_len();
    if (mv == 0) {
      gear_ = 0;
      choice = random() % 3;
    } else {
      if (flip_biased_coin(0.5)) {
        choice = opp_history()[mv];
      } else {
        choice = random() % 3;
      }
      if (mv % 111 == 0) {
        gear_ += 2;
      }
    }
    return ((choice + gear_) % 3);
  }

 private:
  int gear_;
};

}  // namespace roshambo_tournament

#endif  // DRIFTBOT_H
