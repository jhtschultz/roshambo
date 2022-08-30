#ifndef SHOFAR_H
#define SHOFAR_H

#include <cassert>
#include <cmath>
#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Shofar (11)   Rudi Cilibrasi (USA)  */
class Shofar : public RSBBot {
 public:
  static constexpr int kPatternStart = 9;

  struct strat {
    void play(const Shofar &bot) { pname(this, bot); }

    void (*pname)(strat *, const Shofar &);
    double score;
    int move;
    int ivar[16];
  };

  Shofar(int match_length);

  int GetAction() override {
    double base = 1.05;
    double total = 0, r;
    int i;
    if (history_len() == 0) {
      chose_ = -1;
      for (i = 0; i < sc_; ++i) s_[i].score = 0.0;
      for (i = kPatternStart; i < sc_; ++i) pattern_init(&s_[i]);
    } else {
      update_score();
    }
    for (i = 0; i < sc_; ++i) {
      s_[i].play(*this);
      total += pow(base, s_[i].score);
    }
    r = (random() / kMaxRandom) * total;
    for (i = 0; i < sc_; ++i) {
      r -= pow(base, s_[i].score);
      if (r <= 0) break;
    }
    assert(i >= 0 && i < sc_);
    chose_ = i;
    /*      printf("Her move was %d, my move was %d\n",
     * opp_history[opp_history[0]], s[chose].move); */
    return s_[chose_].move;
  }

  void mirror_play(strat *cur) const {
    int hermove = opp_last_move();
    cur->move = (cur->ivar[0] + hermove) % 3;
  }

  void narcissus_play(strat *cur) const {
    int mymove = my_last_move();
    cur->move = (cur->ivar[0] + mymove) % 3;
  }

 private:
  void single_init(strat *cur, int whoiam) const { cur->move = whoiam; }

  void mirror_init(strat *cur, int whoiam) const { cur->ivar[0] = whoiam; }

  void narcissus_init(strat *cur, int whoiam) const { cur->ivar[0] = whoiam; }

  void pattern_init(strat *cur) const {
    int i;
    cur->ivar[0] = 1 + random() / (kMaxRandom / 5);
    cur->ivar[1] = 0;
    for (i = 0; i < cur->ivar[0]; ++i) {
      cur->ivar[i + 2] = 3 * (random() / kMaxRandom);
    }
  }

  void update_score() {
    int i;
    double worstscore = 1000;
    int worstguy;
    int hermove, winmove, losemove;
    worstguy = 0; /* -db */
    hermove = opp_last_move();
    winmove = (hermove + 1) % 3;
    losemove = (hermove + 2) % 3;
    for (i = 0; i < sc_; ++i) {
      int multiplier = (i == chose_) ? 4 : 3;
      if (s_[i].move == winmove)
        s_[i].score += multiplier;
      else if (s_[i].move == losemove)
        s_[i].score -= multiplier;
      s_[i].score *= 0.99;
    }
    worstguy = -1;
    for (i = kPatternStart; i < sc_; ++i)
      if (s_[i].score < worstscore) {
        worstguy = i;
        worstscore = s_[i].score;
      }
    if (worstguy >= 0) pattern_init(&s_[worstguy]);
  }

  struct strat s_[128];
  int sc_ = 0;
  int chose_ = -1;
};

}  // namespace roshambo_tournament

#endif  // SHOFAR_H
