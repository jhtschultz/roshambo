#include "shofar.h"

namespace roshambo_tournament {

namespace {
void single_play(Shofar::strat *cur, const Shofar& bot) {}

void mirror_play_wrapper(Shofar::strat *cur, const Shofar& bot) {
  bot.mirror_play(cur);
}

void narcissus_play_wrapper(Shofar::strat *cur, const Shofar& bot) {
  bot.narcissus_play(cur);
}

void pattern_play(Shofar::strat *cur, const Shofar& bot) {
  cur->move = cur->ivar[cur->ivar[1] + 2];
  cur->ivar[1] = (cur->ivar[1] + 1) % cur->ivar[0];
}
}  // namespace

Shofar::Shofar(int match_length) : RSBBot(match_length) {
  int i;
  for (i = 0; i < 3; ++i) {
    s_[sc_].pname = single_play;
    single_init(&s_[sc_], i);
    ++sc_;
  }
  for (i = 0; i < 3; ++i) {
    s_[sc_].pname = mirror_play_wrapper;
    mirror_init(&s_[sc_], i);
    ++sc_;
  }
  for (i = 0; i < 3; ++i) {
    s_[sc_].pname = narcissus_play_wrapper;
    narcissus_init(&s_[sc_], i);
    ++sc_;
  }
  assert(sc_ == kPatternStart);
  for (i = 0; i < 80; ++i) {
    s_[sc_].pname = pattern_play;
    pattern_init(&s_[sc_]);
    ++sc_;
  }
}

}  // namespace roshambo_tournament
