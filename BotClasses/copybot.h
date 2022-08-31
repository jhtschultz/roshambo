#ifndef COPYBOT_H
#define COPYBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Do whatever would have beat the opponent last turn. */
class CopyBot : public RSBBot {
 public:
  CopyBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override { return (opp_last_move() + 1) % 3; }
};

}  // namespace roshambo_tournament

#endif  // COPYBOT_H
