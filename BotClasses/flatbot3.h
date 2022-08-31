#ifndef FLATBOT3_H
#define FLATBOT3_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/* Flat distribution, 20% chance of most frequent actions */
class FlatBot3 : public RSBBot {
 public:
  FlatBot3(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int mylm, choice;

    choice = 0;
    if (history_len() == 0) {
      rc_ = 0;
      pc_ = 0;
      sc_ = 0;
    } else {
      mylm = my_last_move();
      if (mylm == kRock) {
        ++rc_;
      } else if (mylm == kPaper) {
        ++pc_;
      } else /* mylm == kScissors */ {
        ++sc_;
      }
    }
    if ((rc_ < pc_) && (rc_ < sc_)) {
      choice = biased_roshambo(0.8, 0.1);
    }
    if ((pc_ < rc_) && (pc_ < sc_)) {
      choice = biased_roshambo(0.1, 0.8);
    }
    if ((sc_ < rc_) && (sc_ < pc_)) {
      choice = biased_roshambo(0.1, 0.1);
    }
    if ((rc_ == pc_) && (rc_ < sc_)) {
      choice = biased_roshambo(0.45, 0.45);
    }
    if ((rc_ == sc_) && (rc_ < pc_)) {
      choice = biased_roshambo(0.45, 0.1);
    }
    if ((pc_ == sc_) && (pc_ < rc_)) {
      choice = biased_roshambo(0.1, 0.45);
    }
    if ((rc_ == pc_) && (rc_ == sc_)) {
      choice = random() % 3;
    }
    return choice;
  }

 private:
  int rc_;
  int pc_;
  int sc_;
};

}  // namespace roshambo_tournament

#endif  // FLATBOT3_H
