#ifndef BOT_MAP_H_
#define BOT_MAP_H_

#include <algorithm>
#include <functional>
#include <map>
#include <string>

namespace roshambo_tournament {

extern std::map<std::string, std::function<int()>> bot_map;

}  // namespace roshambo_tournament

#endif  // BOT_MAP_H_
