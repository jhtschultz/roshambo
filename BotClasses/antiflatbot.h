#ifndef ANTIFLATBOT_H
#define ANTIFLATBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Maximally exploit flat distribution */
class AntiFlatBot : public RSBBot {
 public:
  AntiFlatBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int opplm, choice;

    choice = 0;
    if (history_len() == 0) {
      rc_ = 0;
      pc_ = 0;
      sc_ = 0;
    } else {
      opplm = opp_last_move();
      if (opplm == kRock) {
        ++rc_;
      } else if (opplm == kPaper) {
        ++pc_;
      } else /* opplm == kScissors */ {
        ++sc_;
      }
    }
    if ((rc_ < pc_) && (rc_ < sc_)) {
      choice = kPaper;
    }
    if ((pc_ < rc_) && (pc_ < sc_)) {
      choice = kScissors;
    }
    if ((sc_ < rc_) && (sc_ < pc_)) {
      choice = kRock;
    }
    if ((rc_ == pc_) && (rc_ < sc_)) {
      choice = kPaper;
    }
    if ((rc_ == sc_) && (rc_ < pc_)) {
      choice = kRock;
    }
    if ((pc_ == sc_) && (pc_ < rc_)) {
      choice = kScissors;
    }
    if ((rc_ == pc_) && (rc_ == sc_)) {
      choice = random() % 3;
    }
    return (choice);
  }

 private:
  int rc_;
  int pc_;
  int sc_;
};

}  // namespace roshambo_tournament

#endif  // ANTIFLATBOT_H
