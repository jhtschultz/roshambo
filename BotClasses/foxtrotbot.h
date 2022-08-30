#ifndef FOXTROTBOT_H
#define FOXTROTBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Set pattern: rand prev+2 rand prev+1 rand prev+0, repeat */
class FoxtrotBot : public RSBBot {
 public:
  FoxtrotBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    if (int turn = history_len() + 1; turn % 2) {
      return random() % 3;
    } else {
      return (my_history()[turn - 1] + turn) % 3;
    }
  }
};

}  // namespace roshambo_tournament

#endif  // FOXTROTBOT_H
