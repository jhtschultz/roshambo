#include "multibot.h"

namespace roshambo_tournament {

namespace {
int random_md5_wrapper(MultiBot& bot) { return bot.random_md5(); }
int mrockbot_wrapper(MultiBot& bot) { return bot.mrockbot(); }
int mpaperbot_wrapper(MultiBot& bot) { return bot.mpaperbot(); }
int mscissorsbot_wrapper(MultiBot& bot) { return bot.mscissorsbot(); }
int beatcopybot_wrapper(MultiBot& bot) { return bot.beatcopybot(); }
int beatswitchbot_wrapper(MultiBot& bot) { return bot.beatswitchbot(); }
int beatfreqbot_wrapper(MultiBot& bot) { return bot.beatfreqbot(); }
}  // namespace

int MultiBot::GetAction() {
  int i;

  if (FirstTrial()) {
    /* New round */
    strategies.clear();
    strategies.reserve(strategy_count);

    strategies.emplace_back(random_md5_wrapper, AVGLEN);
    strategies.emplace_back(mrockbot_wrapper, AVGLEN);
    strategies.emplace_back(mpaperbot_wrapper, AVGLEN);
    strategies.emplace_back(mscissorsbot_wrapper, AVGLEN);
    strategies.emplace_back(beatcopybot_wrapper, AVGLEN);
    strategies.emplace_back(beatswitchbot_wrapper, AVGLEN);
    strategies.emplace_back(beatfreqbot_wrapper, AVGLEN);

    return random_md5();
  } else {
    /* update sucesses and find best */
    float best = 0.0f;
    int beststrategy = 0;
    int bestmove = 0;
    for (i = 0; i < strategy_count; i++) {
      float success =
          RollingAverage_Add(&strategies[i].success,
                             Score(history_len(), strategies[i].lastmove));
      int move = Strategy_Move(&strategies[i]);

      if (success > best) {
        best = success;
        beststrategy = i;
        bestmove = move;
      }
    }

    /* play the best move */
    return bestmove;
  }
}

}  // namespace roshambo_tournament
