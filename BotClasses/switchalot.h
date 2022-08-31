#ifndef SWITCHALOT_H
#define SWITCHALOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Seldom repeat the previous pick */
class Switchalot : public RSBBot {
 public:
  Switchalot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    if (int my_last = my_last_move(); my_last == kRock) {
      return biased_roshambo(0.12, 0.44);
    } else if (my_last == kPaper) {
      return biased_roshambo(0.44, 0.12);
    } else {
      return biased_roshambo(0.44, 0.44);
    }
  }
};

}  // namespace roshambo_tournament

#endif  // SWITCHALOT_H
