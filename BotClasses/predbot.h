#ifndef PREDBOT_H
#define PREDBOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Simple Predictor (14)   Don Beal (UK)  */
class PredBot : public RSBBot {
 public:
  PredBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int history_length = history_len();
    int i, j, k, l, m, m1, o, o1, s, mr, mp, ms, mb;
    double q, qi, qj, qk, ql;
    if (history_length == 0) {
      for (i = 0; i < 64; i++) c[i] = 0;
      return (random() % 3);
    } else {
      o = opp_history()[history_length];
      m = my_history()[history_length];
      if (history_length > 1) {
        o1 = opp_history()[history_length - 1];
        m1 = my_history()[history_length - 1];
        i = o1 * 16 + m1 * 4 + o;
        j = o1 * 16 + 3 * 4 + o;
        k = 3 * 16 + m1 * 4 + o;
        l = 3 * 16 + 3 * 4 + o;
        c[i] += 1;
        c[j] += 1;
        c[k] += 1;
        c[l] += 1;
        c[i + 3 - o] += 1;
        c[j + 3 - o] += 1;
        c[k + 3 - o] += 1;
        c[l + 3 - o] += 1;
      }
      for (i = 0; i < 64; i++) c[i] = c[i] * 0.98;
      i = o * 16 + m * 4;
      j = o * 16 + 3 * 4;
      k = 3 * 16 + m * 4;
      l = 3 * 16 + 3 * 4;
      for (qi = c[i], m = 1; m < 3; m++)
        if (c[i + m] > qi) qi = c[i + m];
      qi = qi / c[i + 3];
      for (qj = c[j], m = 1; m < 3; m++)
        if (c[j + m] > qj) qj = c[j + m];
      qj = qj / c[j + 3];
      for (qk = c[k], m = 1; m < 3; m++)
        if (c[k + m] > qk) qk = c[k + m];
      qk = qk / c[k + 3];
      for (ql = c[l], m = 1; m < 3; m++)
        if (c[l + m] > ql) ql = c[l + m];
      ql = ql / c[l + 3];
      q = qi;
      s = i;
      if (qj > q) {
        q = qj;
        s = j;
      }
      if (qk > q) {
        q = qk;
        s = k;
      }
      if (ql > q) {
        s = l;
      }
      mr = c[s + 2] - c[s + 1];
      mp = c[s] - c[s + 2];
      ms = c[s + 1] - c[s];
      mb = mr;
      m = kRock;
      if (mp > mb) {
        mb = mp;
        m = kPaper;
      }
      if (ms > mb) m = kScissors;
      return (m);
    }
  }

 private:
  double c[64];
};

}  // namespace roshambo_tournament

#endif  // PREDBOT_H
