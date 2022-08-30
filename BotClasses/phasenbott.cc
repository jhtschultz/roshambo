#include "phasenbott.h"

namespace roshambo_tournament {
namespace {
long jlmhist0_wrapper(Phasenbott& bot) { return bot.jlmhist0(); }

long jlmhist1_wrapper(Phasenbott& bot) { return bot.jlmhist1(); }

long jlmrand_wrapper(Phasenbott& bot) { return bot.jlmrand(); }

long apply_jocaine_wrapper(Phasenbott& bot) {
  return bot.apply_jocaine_inner_wrapper();
}
}  // namespace

Phasenbott::Phasenbott(int match_length) : RSBBot(match_length) {
  t_ = {jlmbot(jlmhist0_wrapper)};
  s_ = {jlmbot(jlmhist1_wrapper), jlmbot(jlmhist0_wrapper),
    jlmbot(jlmrand_wrapper), jlmbot(apply_jocaine_wrapper)};
}

}  // namespace roshambo_tournament
