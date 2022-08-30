#ifndef R226BOT_H
#define R226BOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Plays 20% rock, 20% paper, 60% scissors. */
class R226Bot : public RSBBot {
 public:
  R226Bot(int match_length) : RSBBot(match_length) {}

  int GetAction() override { return biased_roshambo(0.2, 0.2); }
};

}  // namespace roshambo_tournament

#endif  // R226BOT_H
