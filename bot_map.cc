#include "bot_map.h"

#include <cassert>

#include "BotClasses/actr_lag2_decay.h"
#include "BotClasses/adddriftbot2.h"
#include "BotClasses/addshiftbot3.h"
#include "BotClasses/antiflatbot.h"
#include "BotClasses/antirotnbot.h"
#include "BotClasses/biopic.h"
#include "BotClasses/boom.h"
#include "BotClasses/copybot.h"
#include "BotClasses/debruijn81.h"
#include "BotClasses/driftbot.h"
#include "BotClasses/flatbot3.h"
#include "BotClasses/foxtrotbot.h"
#include "BotClasses/freqbot.h"
#include "BotClasses/granite.h"
#include "BotClasses/greenberg.h"
#include "BotClasses/halbot.h"
#include "BotClasses/inocencio.h"
#include "BotClasses/iocainebot.h"
#include "BotClasses/marble.h"
#include "BotClasses/markov5.h"
#include "BotClasses/mixed_strategy.h"
#include "BotClasses/mod1bot.h"
#include "BotClasses/multibot.h"
#include "BotClasses/peterbot.h"
#include "BotClasses/phasenbott.h"
#include "BotClasses/pibot.h"
#include "BotClasses/piedra.h"
#include "BotClasses/predbot.h"
#include "BotClasses/r226bot.h"
#include "BotClasses/randbot.h"
#include "BotClasses/robertot.h"
#include "BotClasses/rockbot.h"
#include "BotClasses/rotatebot.h"
#include "BotClasses/russrocker4.h"
#include "BotClasses/shofar.h"
#include "BotClasses/suncrazybot.h"
#include "BotClasses/sunnervebot.h"
#include "BotClasses/sweetrock.h"
#include "BotClasses/switchalot.h"
#include "BotClasses/switchbot.h"
#include "BotClasses/textbot.h"
#include "BotClasses/zqmove.h"

namespace roshambo_tournament {

std::map<std::string, std::unique_ptr<RSBBot> (*)(int)> bot_map = {
    {"randbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<RandBot>(num_throws);
     }},
    {"rockbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<RockBot>(num_throws);
     }},
    {"r226bot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<R226Bot>(num_throws);
     }},
    {"rotatebot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<RotateBot>(num_throws);
     }},
    {"copybot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<CopyBot>(num_throws);
     }},
    {"switchbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<SwitchBot>(num_throws);
     }},
    {"freqbot2",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<FreqBot>(num_throws);
     }},
    {"pibot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<PiBot>(num_throws);
     }},
    {"switchalot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Switchalot>(num_throws);
     }},
    {"flatbot3",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<FlatBot3>(num_throws);
     }},
    {"antiflatbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<AntiFlatBot>(num_throws);
     }},
    {"foxtrotbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<FoxtrotBot>(num_throws);
     }},
    {"debruijn81",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<DeBruijn81>(num_throws);
     }},
    {"textbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<TextBot>(num_throws);
     }},
    {"antirotnbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<AntiRotnBot>(num_throws);
     }},
    {"driftbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<DriftBot>(num_throws);
     }},
    {"addshiftbot3",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<AddshiftBot3>(num_throws);
     }},
    {"adddriftbot2",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<AdddriftBot2>(num_throws);
     }},
    {"iocainebot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<IocaineBot>(num_throws);
     }},
    {"phasenbott",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Phasenbott>(num_throws);
     }},
    {"halbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<HalBot>(num_throws);
     }},
    {"russrocker4",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<RussRocker4>(num_throws);
     }},
    {"biopic",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Biopic>(num_throws);
     }},
    {"mod1bot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Mod1Bot>(num_throws);
     }},
    {"predbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<PredBot>(num_throws);
     }},
    {"robertot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Robertot>(num_throws);
     }},
    {"boom",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Boom>(num_throws);
     }},
    {"shofar",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Shofar>(num_throws);
     }},
    {"actr_lag2_decay",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<ActrLag2Decay>(num_throws);
     }},
    {"markov5",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Markov5>(num_throws, false);
     }},
    {"markovbails",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Markov5>(num_throws, true);
     }},
    {"granite",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Granite>(num_throws);
     }},
    {"marble",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Marble>(num_throws);
     }},
    {"zq_move",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<ZQMove>(num_throws);
     }},
    {"sweetrock",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<SweetRock>(num_throws);
     }},
    {"piedra",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Piedra>(num_throws);
     }},
    {"mixed_strategy",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<MixedStrategy>(num_throws);
     }},
    {"multibot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<MultiBot>(num_throws);
     }},
    {"inocencio",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Inocencio>(num_throws);
     }},
    {"peterbot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<PeterBot>(num_throws);
     }},
    {"sunNervebot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<SunNervebot>(num_throws);
     }},
    {"sunCrazybot",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<SunCrazybot>(num_throws);
     }},
    {"greenberg",
     [](int num_throws) -> std::unique_ptr<RSBBot> {
       return std::make_unique<Greenberg>(num_throws);
     }},
};

}  // namespace roshambo_tournament
