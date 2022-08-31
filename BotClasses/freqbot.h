#ifndef FREQBOT_H
#define FREQBOT_H

#include <algorithm>
#include <array>
#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Beat Frequent Pick */
class FreqBot : public RSBBot {
 public:
  FreqBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    if (history_len() == 0) {
      counts_.fill(0);
    } else {
      ++counts_[opp_last_move()];
    }
    auto max_it = std::max_element(counts_.rbegin(), counts_.rend());
    int freq_action = std::distance(max_it, counts_.rend()) - 1;
    return (freq_action + 1) % 3;
  }

 private:
  std::array<int, 3> counts_;
};

}  // namespace roshambo_tournament

#endif  // FREQBOT_H
