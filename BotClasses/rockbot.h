#ifndef ROCKBOT_H
#define ROCKBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* "Good ole rock.  Nuthin' beats rock." */
class RockBot : public RSBBot {
 public:
  RockBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override { return kRock; }
};

}  // namespace roshambo_tournament

#endif  // ROCKBOT_H
