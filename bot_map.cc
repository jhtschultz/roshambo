#include "open_spiel/bots/roshambo/roshambo/bot_map.h"

namespace roshambo_tournament {

extern "C" {
  int randbot();
  int rockbot();
  int r226bot();
  int rotatebot();
  int copybot();
  int switchbot();
  int freqbot2();
  int pibot();
  int switchalot();
  int flatbot3();
  int antiflatbot();
  int foxtrotbot();
  int debruijn81();
  int textbot();
  int antirotnbot();
  int driftbot();
  int addshiftbot3();
  int adddriftbot2();
  int iocainebot();
  int phasenbott();
  int halbot();
  int russrocker4();
  int biopic();
  int mod1bot();
  int predbot();
  int robertot();
  int boom();
  int shofar();
  int actr_lag2_decay();
  int markov5();
  int markovbails();
  int granite();
  int marble();
  int zq_move();
  int sweetrock();
  int piedra();
  int mixed_strategy();
  int multibot();
  int inocencio();
  int peterbot();
  int sunNervebot();
  int sunCrazybot();
  int greenberg();
}

std::map<std::string, std::function<int()>> bot_map = {
    {"randbot", randbot},
    {"rockbot", rockbot},
    {"r226bot", r226bot},
    {"rotatebot", rotatebot},
    {"copybot", copybot},
    {"switchbot", switchbot},
    {"freqbot2", freqbot2},
    {"pibot", pibot},
    {"switchalot", switchalot},
    {"flatbot3", flatbot3},
    {"antiflatbot", antiflatbot},
    {"foxtrotbot", foxtrotbot},
    {"debruijn81", debruijn81},
    {"textbot", textbot},
    {"antirotnbot", antirotnbot},
    {"driftbot", driftbot},
    {"addshiftbot3", addshiftbot3},
    {"adddriftbot2", adddriftbot2},
    {"iocainebot", iocainebot},
    {"phasenbott", phasenbott},
    {"halbot", halbot},
    {"russrocker4", russrocker4},
    {"biopic", biopic},
    {"mod1bot", mod1bot},
    {"predbot", predbot},
    {"robertot", robertot},
    {"boom", boom},
    {"shofar", shofar},
    {"actr_lag2_decay", actr_lag2_decay},
    {"markov5", markov5},
    {"markovbails", markovbails},
    {"granite", granite},
    {"marble", marble},
    {"zq_move", zq_move},
    {"sweetrock", sweetrock},
    {"piedra", piedra},
    {"mixed_strategy", mixed_strategy},
    {"multibot", multibot},
    {"inocencio", inocencio},
    {"peterbot", peterbot},
    {"sunNervebot", sunNervebot},
    {"sunCrazybot", sunCrazybot},
    {"greenberg", greenberg},
};

}  // namespace roshambo_tournament
