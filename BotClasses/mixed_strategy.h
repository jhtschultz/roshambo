#ifndef MIXED_STRATEGY_H
#define MIXED_STRATEGY_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Mixed Strategy (28)   Thad Frogley (UK)

   > I also welcome more feedback from the participants, both
   > on your ideas and on your personal background.

 Darse,

 As I said in an earlier mail I thought it was a very well run tournament,
 and I will be entering the second one, probably with an cleaner/faster
 "mixed strategy" bot, plus a new one that's been busting my brain since I
 read about "Iocaine Powder" (so do I play what will beat what I predict they
 will play, or do I play what will beat what will beat what I predict that
 they predict what I will play?  Ungk.  Fizzle.).

 Anywho, ask you asked, my info:

 I am:
 Thad (Thaddaeus Frogley) 24 years old, programmer for CLabs/CyberLife
 Technology Ltd, Cambridge, England
 No university education, self taught programmer (from ooh around the age of
 7 hmmm zx81 with 16k ram pack!).
 Maintainer (but not very active) of the Robot Battle FAQ
 [http://www.robotbattle.com]
 Oh, and some people seem to find it interesting that I'm dyslexic.

 My bot is:
 Mixed Strategy, and is *not* based on the CLabs alife philosophy, it is
 instead based on my experiences with Robot Battle (RB) where the Muli-mode
 Bot / Mode Switcher / Meta Bot is a well established tactic.

 Initially I thought that simply wrapping the built in behaviours in a basic
 analysing mode switcher would be enough to do well in the tournament (I
 seriously under estimated the calibre of the participants), but then after
 brief correspondence with your good self I got that nagging feeling that I
 needed to do more.  Due to time constraints I limited my changes to the
 creation of two 'new' behaviours based on pair wise statistical probability
 predication (named watching-you and watching-you-watching-me) and stripped
 out the modes that I felt where redundant.  In hindsight I have realised
 that I could have probably removed the "Beat Frequent Pick" and "random"
 modes leaving the random factor to the context switching between the
 remaining modes.

 I have some other ideas for improving Mixed Strategy, and I have an idea for
 a new bot, but I'll save them for next time.

 I'll close by pointing out the following issues common to all mode based
 adaptive AIs.

 1) The learning curve.  To many modes means to much time spend learning, and
 not enough winning.
 2) Mode locking.  Without a decay function a mode that is adapted against
 will loose as many as it won before switching.  (Hence the decay function in
 my one.)

 I hope this lot adds to everybody's knowledge and enjoyment of the game!

 Thad
*/
class MixedStrategy : public RSBBot {
 public:
  MixedStrategy(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int i, rcount, pcount, scount;

    int turn = history_len();
    double t;

    if (turn == 0) {
      strategy_scores[0] = 4; /* watching you watching me */
      strategy_scores[1] = 4; /* watching you  */
      strategy_scores[2] = 2; /* freqbot */
      strategy_scores[3] = 1; /* random */
    } else {
      /* remeber success of prev stratigies */
      if (my_history()[turn] == opp_history()[turn]) {
        strategy_scores[last_strategy] += 1; /* draw */
      } else if ((my_history()[turn] - opp_history()[turn] == 1) ||
                 (my_history()[turn] - opp_history()[turn] == -2)) {
        strategy_scores[last_strategy] += 3; /* win (test from Play_Match) */
      } else {
        strategy_scores[last_strategy] =
            (int)((double)strategy_scores[last_strategy] * 0.8);
      }
    }

    /* pick based on rate of success for each strategy */
    t = random();
    t /= kMaxRandom;
    t *= (strategy_scores[0] + strategy_scores[1] + strategy_scores[2] +
          strategy_scores[3]);

    if (t < strategy_scores[0]) {
      last_strategy = 0;
      /* play whatever will beat the opponent's most frequent follow up to
         my last move */

      rcount = 0;
      pcount = 0;
      scount = 0;
      for (i = 2; i <= history_len() - 1; i++) {
        if (my_history()[i - 1] == my_last_move()) {
          if (opp_history()[i] == kRock) {
            rcount++;
          } else if (opp_history()[i] == kPaper) {
            pcount++;
          } else /* opp_history()[i] == kScissors */ {
            scount++;
          }
        }
      }
      if ((rcount > pcount) && (rcount > scount)) {
        return (kPaper);
      } else if (pcount > scount) {
        return (kScissors);
      } else {
        return (kRock);
      }
    } else if (t < strategy_scores[0] + strategy_scores[1]) { /* note change */
      last_strategy = 1;
      /* play whatever will beat the opponent's most frequent follow up to his
         last move */

      rcount = 0;
      pcount = 0;
      scount = 0;
      for (i = 2; i <= history_len() - 1; i++) {
        if (opp_history()[i - 1] == opp_last_move()) {
          if (opp_history()[i] == kRock) {
            rcount++;
          } else if (opp_history()[i] == kPaper) {
            pcount++;
          } else /* opp_history()[i] == kScissors */ {
            scount++;
          }
        }
      }
      if ((rcount > pcount) && (rcount > scount)) {
        return (kPaper);
      } else if (pcount > scount) {
        return (kScissors);
      } else {
        return (kRock);
      }

    } else if (t <
               strategy_scores[0] + strategy_scores[1] + strategy_scores[2]) {
      last_strategy = 2;
      /* play whatever will beat the opponent's most frequent choice */

      rcount = 0;
      pcount = 0;
      scount = 0;
      for (i = 1; i <= history_len(); i++) {
        if (opp_history()[i] == kRock) {
          rcount++;
        } else if (opp_history()[i] == kPaper) {
          pcount++;
        } else /* opp_history()[i] == kScissors */ {
          scount++;
        }
      }
      if ((rcount > pcount) && (rcount > scount)) {
        return (kPaper);
      } else if (pcount > scount) {
        return (kScissors);
      } else {
        return (kRock);
      }
    } else {
      last_strategy = 3;
      return (random() % 3);
    }
  }

 private:
  int strategy_scores[4];
  int last_strategy;
};

}  // namespace roshambo_tournament

#endif  // MIXED_STRATEGY_H
