#ifndef SWITCHBOT_H
#define SWITCHBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Never repeat the previous pick */
class SwitchBot : public RSBBot {
 public:
  SwitchBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    if (int my_last = my_last_move(); my_last == kRock) {
      return biased_roshambo(0.0, 0.5);
    } else if (my_last == kPaper) {
      return biased_roshambo(0.5, 0.0);
    } else {
      return biased_roshambo(0.5, 0.5);
    }
  }
};

}  // namespace roshambo_tournament

#endif  // SWITCHBOT_H
