#ifndef RANDBOT_H
#define RANDBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Generate action uniformly at random (optimal strategy). */
class RandBot : public RSBBot {
 public:
  RandBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override { return random() % 3; }
};

}  // namespace roshambo_tournament

#endif  // RANDBOT_H
