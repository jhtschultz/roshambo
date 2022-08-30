#ifndef ROTATEBOT_H
#define ROTATEBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Rotate choice each turn r -> p -> s. */
class RotateBot : public RSBBot {
 public:
  RotateBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override { return history_len() % 3; }
};

}  // namespace roshambo_tournament

#endif  // ROTATEBOT_H
