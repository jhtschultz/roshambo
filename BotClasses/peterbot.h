#ifndef PETERBOT_H
#define PETERBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Peterbot (50)   Peter Baylie (USA)  */
class PeterBot : public RSBBot {
 public:
  PeterBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    /* maintain stats with static variables to avoid re-scanning the
       history array */
    int opp_last, opp_prev, my_last, my_prev, myfreq, i;

    opp_prev = 0;
    my_prev = 0; /* -db */
    if (history_len() == 0) {
      oc.r = 0;
      oc.p = 0;
      oc.s = 0;
      opp_last = random() % 3;
      opp_prev = random() % 3;
    } else {
      opp_last = opp_last_move();
      if (history_len() != 1) opp_prev = opp_history()[history_len() - 1];
      if (opp_last == kRock) {
        oc.r++;
      } else if (opp_last == kPaper) {
        oc.p++;
      } else {
        oc.s++;
      }
    }

    if (history_len() == 0) {
      mc.r = 0;
      mc.p = 0;
      mc.s = 0;
      my_last = random() % 3;
      my_prev = random() % 3;
    } else {
      my_last = my_last_move();
      if (history_len() != 1) my_prev = my_history()[history_len() - 1];
      if (my_last == kRock) {
        mc.r++;
      } else if (my_last == kPaper) {
        mc.p++;
      } else {
        mc.s++;
      }
    }

    /* beat stupid */
    if ((oc.r - oc.p - oc.s) > 0) {
      return (kPaper);
    } else if ((oc.p - oc.r - oc.s) > 0) {
      return (kScissors);
    } else if ((oc.s - oc.p - oc.r) > 0) {
      return (kRock);
    }

    /* beat rotate */
    i = history_len() - 50;
    if (i < 0) i = 1;
    while ((i < history_len()) &&
           ((opp_history()[i] + 1) % 3 == opp_history()[i + 1]))
      i++;
    if (i == history_len()) {
      return ((opp_history()[i] + 2) % 3);
    };

    /* beat freq */
    if ((mc.r > mc.p) && (mc.r > mc.s))
      myfreq = kPaper;
    else if (mc.p > mc.s)
      myfreq = kScissors;
    else
      myfreq = kRock;
    if (myfreq == opp_last && myfreq == opp_prev) {
      return (opp_last + 1) % 3;
    };

    /* beat switching */
    if (opp_last != opp_prev) {
      i = 0;
      while ((i == opp_last) || (i == opp_prev)) i++;
      return (i + 1) % 3;
    };

    /* beat last */
    if (opp_last == (my_prev + 1) % 3) {
      return (my_last + 2) % 3;
    };

    /* be random */
    return random() % 3;
  }

 private:
  struct tri {
    int r, p, s;
  } oc, mc;
};

}  // namespace roshambo_tournament

#endif  // PETERBOT_H
