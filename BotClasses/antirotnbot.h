#ifndef ANTIROTNBOT_H
#define ANTIROTNBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Observes rotations in opponent's sequence, exploits max or min, whichever
 * difference is greater */
class AntiRotnBot : public RSBBot {
 public:
  AntiRotnBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int mv, diff, diff2, small, med, large;

    mv = history_len();
    if (mv == 0) {
      no_ = 0;
      up_ = 0;
      dn_ = 0;
      score_ = 0;
    } else {
      diff = (my_history()[mv] - opp_history()[mv] + 3) % 3;
      if (diff == 1) {
        score_++;
      }
      if (diff == 2) {
        score_--;
      }
      if (mv > 1) {
        diff = (opp_history()[mv] - opp_history()[mv - 1] + 3) % 3;
        if (diff == 0) {
          no_++;
        }
        if (diff == 1) {
          up_++;
        }
        if (diff == 2) {
          dn_++;
        }
      }
    }

    /* fail-safe at 4% of match length */
    if (score_ < -rsb_trials() / 25) {
      return (random() % 3);
    }

    if ((no_ == up_) && (no_ == dn_)) {
      return (random() % 3);
    }

    /* sort */
    if ((no_ <= up_) && (no_ <= dn_)) {
      small = no_;
      if (up_ <= dn_) {
        med = up_;
        large = dn_;
      } else {
        med = dn_;
        large = up_;
      }
    } else if (up_ <= dn_) {
      small = up_;
      if (no_ <= dn_) {
        med = no_;
        large = dn_;
      } else {
        med = dn_;
        large = no_;
      }
    } else {
      small = dn_;
      if (no_ <= up_) {
        med = no_;
        large = up_;
      } else {
        med = up_;
        large = no_;
      }
    }

    diff = med - small;
    diff2 = large - med;

    if (diff < diff2) { /* clear maximum */
      if ((no_ > up_) && (no_ > dn_)) {
        return ((opp_last_move() + 1) % 3);
      }
      if ((up_ > no_) && (up_ > dn_)) {
        return ((opp_last_move() + 2) % 3);
      }
      if ((dn_ > no_) && (dn_ > up_)) {
        return (opp_last_move());
      }
    } else if (diff > diff2) { /* clear minimum */
      if ((dn_ < up_) && (dn_ < no_)) {
        return ((opp_last_move() + 1) % 3);
      }
      if ((up_ < dn_) && (up_ < no_)) {
        return (opp_last_move());
      }
      if ((no_ < dn_) && (no_ < up_)) {
        return ((opp_last_move() + 2) % 3);
      }
    } else if (diff == diff2) {
      if ((no_ > up_) && (up_ > dn_)) {
        return ((opp_last_move() + 1) % 3);
      }
      if ((dn_ > up_) && (up_ > no_)) {
        if (flip_biased_coin(0.5)) {
          return (opp_last_move());
        } else {
          return ((opp_last_move() + 2) % 3);
        }
      }
      if ((dn_ > no_) && (no_ > up_)) {
        return (opp_last_move());
      }
      if ((up_ > no_) && (no_ > dn_)) {
        if (flip_biased_coin(0.5)) {
          return ((opp_last_move() + 1) % 3);
        } else {
          return ((opp_last_move() + 2) % 3);
        }
      }
      if ((up_ > dn_) && (dn_ > no_)) {
        return ((opp_last_move() + 2) % 3);
      }
      if ((no_ > dn_) && (dn_ > up_)) {
        if (flip_biased_coin(0.5)) {
          return (opp_last_move());
        } else {
          return ((opp_last_move() + 1) % 3);
        }
      }
    }
    // Error in antirotnbot decision tree!
    return (0);
  }

 private:
  int no_, up_, dn_, score_;
};

}  // namespace roshambo_tournament

#endif  // ANTIROTNBOT_H
