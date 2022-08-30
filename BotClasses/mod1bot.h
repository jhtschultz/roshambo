#ifndef MOD1BOT_H
#define MOD1BOT_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Simple Modeller (7)   Don Beal (UK)

 The simple predictor counts the number of times r/p/s occurred
 after each of the possible move events.  In addition to the 9
 possible r/p/s combinations for the two players, counts are kept
 ignoring the player move, ignoring the opponent move, or both,
 leading to 16 sets of counts.  The predictor then selects the
 count set that has the most extreme distribution, and plays
 against that.  The play against a given distribution is obtained
 by calculating the expected return of each play, and selecting the
 play with the best return.

 The idea to select the most extreme distribution (instead of the
 distribution in which we have the greatest confidence) was an
 experiment - I thought it might promote information gathering
 plays, and aggressively exploit easily-predictable opponents
 earlier than cautious approaches would.  It had the accidental
 advantage of being harder to predict!

 The simple modeller keeps the same information as the simple
 predictor, but for both players.  It can therefore counter an
 opponent using a similar counting technique.  To choose the count
 set to play against, the simple modeller keeps track of past
 performance (the score we would have if we had used that count set
 for all the moves so far).  The simple modeller then plays against
 the count set with the highest score.  If no count set shows a
 positive score, it plays at random.

 Both programs exponentially decay their memory of past plays to
 improve performance against opponents who change their strategy
 over time.

 [Both programs were written hastily and not very readably - sorry
 about that!]
  --
  Don Beal
*/
class Mod1Bot : public RSBBot {
 public:
  static constexpr double fade = 0.98;

  Mod1Bot(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int bm;
    int i, j, k, l, m, m1, o, o1, s, id;
    double q, qi, qj, qk, ql, qd;
    int history_length = history_len();
    m = 0;
    m1 = 0;
    o = 0;
    o1 = 0; /* -db */
    if (history_length > 0) {
      o = opp_history()[history_length];
      m = my_history()[history_length];
    }
    if (history_length > 1) {
      o1 = opp_history()[history_length - 1];
      m1 = my_history()[history_length - 1];
    }
    if (history_length == 0) {
      for (i = 0; i < 96; i++) c_[i] = d_[i] = 0;
    }
    if (history_length > 1) {
      i = o1 * 24 + m1 * 6;
      j = o1 * 24 + 3 * 6;
      k = 3 * 24 + m1 * 6;
      l = 3 * 24 + 3 * 6;
      if (history_length > 2) {
        if (c_[i + 3] > 0)
        /* c[i+4]+=score(bplay(&c[i]),o); */
        {
          SETC(i);
          SETU();
          bm = SETBM();
          c_[i + 4] += SCORE(bm, o);
          c_[i + 5] += 1;
        }
        if (c_[j + 3] > 0) {
          SETC(j);
          SETU();
          bm = SETBM();
          c_[j + 4] += SCORE(bm, o);
          c_[j + 5] += 1;
        }
        if (c_[k + 3] > 0) {
          SETC(k);
          SETU();
          bm = SETBM();
          c_[k + 4] += SCORE(bm, o);
          c_[k + 5] += 1;
        }
        if (c_[l + 3] > 0) {
          SETC(l);
          SETU();
          bm = SETBM();
          c_[l + 4] += SCORE(bm, o);
          c_[l + 5] += 1;
        }
      }
      c_[i + o] += 1;
      c_[j + o] += 1;
      c_[k + o] += 1;
      c_[l + o] += 1;
      c_[i + 3] += 1;
      c_[j + 3] += 1;
      c_[k + 3] += 1;
      c_[l + 3] += 1;
      i = m1 * 24 + o1 * 6;
      j = m1 * 24 + 3 * 6;
      k = 3 * 24 + o1 * 6;
      l = 3 * 24 + 3 * 6;
      if (history_length > 2) {
        if (d_[i + 3] > 0)
        /* md=bplay(&d_[i]); d_[i+4]+=score((md+1)%3,o); */
        {
          SETD(i);
          SETU();
          bm = SETBM();
          bm = (bm + 1) % 3;
          d_[i + 4] += SCORE(bm, o);
          d_[i + 5] += 1;
        }
        if (d_[j + 3] > 0) {
          SETD(j);
          SETU();
          bm = SETBM();
          bm = (bm + 1) % 3;
          d_[j + 4] += SCORE(bm, o);
          d_[j + 5] += 1;
        }
        if (d_[k + 3] > 0) {
          SETD(k);
          SETU();
          bm = SETBM();
          bm = (bm + 1) % 3;
          d_[k + 4] += SCORE(bm, o);
          d_[k + 5] += 1;
        }
        if (d_[l + 3] > 0) {
          SETD(l);
          SETU();
          bm = SETBM();
          bm = (bm + 1) % 3;
          d_[l + 4] += SCORE(bm, o);
          d_[l + 5] += 1;
        }
      }
      d_[i + m] += 1;
      d_[j + m] += 1;
      d_[k + m] += 1;
      d_[l + m] += 1;
      d_[i + 3] += 1;
      d_[j + 3] += 1;
      d_[k + 3] += 1;
      d_[l + 3] += 1;
    }
    if (history_length > 50)
      for (i = 0; i < 96; i++) {
        c_[i] = c_[i] * fade;
        d_[i] = d_[i] * fade;
      }
    if (history_length == 0)
      return (random() % 3);
    else if (history_length == 1)
      return ((o + 1) % 3);
    else {
      id = m * 24 + o * 6;
      SETD(id);
      SETU();
      bm = SETBM();
      bm = (bm + 1) % 3;
      qd = d_[id + 5] > 0 ? d_[id + 4] / d_[id + 5] : 0;
      i = o * 24 + m * 6;
      j = o * 24 + 3 * 6;
      k = 3 * 24 + m * 6;
      l = 3 * 24 + 3 * 6;
      qi = c_[i + 5] > 0 ? c_[i + 4] / c_[i + 5] : 0;
      qj = c_[j + 5] > 0 ? c_[j + 4] / c_[j + 5] : 0;
      qk = c_[k + 5] > 0 ? c_[k + 4] / c_[k + 5] : 0;
      ql = c_[l + 5] > 0 ? c_[l + 4] / c_[l + 5] : 0;
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
        q = ql;
        s = l;
      }
      if (qd > q && qd > 0) return (bm);
      SETC(s);
      SETU();
      bm = SETBM();
      if (q > 0) return (bm);
      return (random() % 3);
    }
  }

 private:
  void SETC(int x) {
    c0_ = c_[x];
    c1_ = c_[x + 1];
    c2_ = c_[x + 2];
  }

  void SETD(int x) {
    c0_ = d_[x];
    c1_ = d_[x + 1];
    c2_ = d_[x + 2];
  }

  void SETU() {
    u0_ = c2_ - c1_;
    u1_ = c0_ - c2_;
    u2_ = c1_ - c0_;
  }

  int SETBM() {
    double b = u0_;
    int bm = 0;
    if (u1_ > b) {
      b = u1_;
      bm = 1;
    }
    if (u2_ > b) {
      b = u2_;
      bm = 2;
    }
    return bm;
  }
  static int SCORE(int m, int o) {
    return m == o ? 0 : (((o + 1) % 3) == m ? 1 : -1);
  }

  double c_[96];
  double d_[96];
  double c0_;
  double c1_;
  double c2_;
  double u0_;
  double u1_;
  double u2_;
};

}  // namespace roshambo_tournament

#endif  // MOD1BOT_H
