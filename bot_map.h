#ifndef BOT_MAP_H_
#define BOT_MAP_H_

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "BotClasses/rsb_bot.h"

namespace roshambo_tournament {

extern std::map<std::string, std::unique_ptr<RSBBot> (*)(int)> bot_map;

}  // namespace roshambo_tournament

#endif  // BOT_MAP_H_
