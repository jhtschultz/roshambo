#ifndef PHASENBOTT_H
#define PHASENBOTT_H

#include <cstdlib>
#include <vector>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Phasenbott (2)   Jakob Mandelson (USA)

 Phasenbott uses a similar strategy to Dan Egnor's "Iocaine Powder",
 indeed it is derived from an early version of Dan's work.

 The early Iocaine Powder ("Old IP") took a single strategy as input, and
 "conjugated" it into six strategies:
   1. Play the strategy.
   2. Assume opponent thinks you'll play the strategy, and beat that.
   3. Assume opponent thinks you'll do (2), and beat that.
   4. Assume opponent plays strategy, and beat that.
   5. Assume opponent thinks you'll do (4), and beat that.
   6. Assume opponent thinks you'll do (5), and beat that.

 Because of the circular nature of the Rock beats Scissors beats Paper
 beats Rock, if you assume the opponent thinks you'll do (3) then you'll
 play (1), and if you assume the opponent thinks you'll do (6) then you'll
 play (4), so these six "strategies" subsume a whole slew of second-guessing.
 (All assuming the initial strategy, though.)  Then it counts which one would
 have done best historically if played, and chooses that strategy to play.
 Old IP used a history match as its base strategy.

 I generalised this concept into a function which took in an array of
 "bots" (each of which returns what it would play if it were you, and if
 it were your opponent), did the six-way conjugation on each, and chose the
 best strategy of those to play.  Note that this function is itself a "bot",
 and can be fed into itself.  If you consider the operator I to take
 strategies and choose the best among their conjugates, then Old IP can be
 represented by I(History).  Phasenbott uses a alternate history mechanism
 which performed better, in addition to Dan's original history mechanism and
 Old IP and Random (as a fallback) for its set of base strategies.
 Phasenbott=I(History, AltHistory, Old IP, Random)

 "New" IP (Dan's winning program) effectly subsumes Phasenbott, so it's
 no surprise that Phasenbott lost to it.  It uses the new history mechanism
 in addition to the original one, like Phasenbott, and also incorporates
 frequency analysis.  In addition, at the very end after it's applied the
 various strategies, it applies the I operator to the result!  This means
 it's effectively checking to see how well it would do by playing to beat/lose
 to itself, and playing that if it is better.  I've checked the effects of
 this "meta-ing", and after the first couple steps it's not worthwhile:
 If one of the base strategies doesn't match the opponent's play, then
 Iocaine's strategy becomes so subtle as to be effectively random.  If one
 of the base strategies does associate with the opponent, then the meta-ing
 does no good.

 Iocaine Powder also uses a more accurate metric for comparing the strategies.
 Where Phasenbott plays the strategy that results in the most wins, Iocaine
 Powder takes draws (or losses, depending on your POV) into account, and plays
 the highest scorer.  Phasenbott's metric would be more appropriate in a
 non-zero sum Rock-Paper-Scissors game where one simply tallied points for
 wins.  This game is more interesting from the theoretical standpoint, as
 there is now incentive for cooperation and no longer a single optimal
 strategy.  Random scores an expected 1/3, but cooperating players could
 do better by alternating wins, for 1/2.  A player wanting to do better than
 1/2 would try to exploit the other player, but not enough that the other
 player detects that it's worthwhile to switch into Random mode.  The weak
 player scoring say 2/5 could know that it's being exploited by the stronger,
 but still go along with it as if it refused (by going Random) its score would
 drop to 1/3.  This in my mind makes for a much more interesting
 Rock-Paper-Scissors game to study than "Roshambo".  Maybe the next
 Rock-Paper-Scissors programming contest will feature such a non-zero sum
 game.  [Hint, hint. :) ]
 */
/*  Phasenbott  --   Jakob Mandelson.
 *    Roshambo competition program
 *    Looks at a series of strategies, and compares how they did historically.
 *    Then plays one that would have played best.
 *    Strategies used: Historical prediction based on sequence of both parties,
 *      and of one party, and itself using only both-party history prediction.
 *    Based on early Iocaine Powder bot of Dan Egnor of playing strategy
 *      that would have done best historically.  Used with permission,
 *      and some ideas crossed back into Dan's Iocaine.
 *    May the best program (more likely Dan's than mine :) win!
 */
class Phasenbott : public RSBBot {
 public:
  Phasenbott(int match_length);

  int GetAction() override { return apply_jocaine(s_) & 0xFFFF; }

  long jlmhist1() {
    jlm_history();
    if (0 == h_.opp) return biased_roshambo(1.0 / 3.0, 1.0 / 3.0);
    return pwill_beat(opp_history()[h_.opp + 1]) |
           (pwill_beat(my_history()[h_.my + 1]) << 16);
  }

  long jlmhist0() {
    jlm_history();
    if (0 == h_.both) return biased_roshambo(1.0 / 3.0, 1.0 / 3.0);
    return pwill_beat(opp_history()[h_.both + 1]) |
           (pwill_beat(my_history()[h_.both + 1]) << 16);
  }

  long jlmrand() {
    /* Fallback to keep from losing too badly.  */
    return biased_roshambo(1.0 / 3.0, 1.0 / 3.0) |
           (biased_roshambo(1.0 / 3.0, 1.0 / 3.0) << 16);
  }

  long apply_jocaine_inner_wrapper() { return apply_jocaine(t_); }

 private:
  struct jlmbot {
    jlmbot(long (*to_beat)(Phasenbott&)) : to_beat_fcn(to_beat) {}

    long (*to_beat_fcn)(Phasenbott&);
    int my_last;
    int opp_last;
    int my_stats[3];
    int opp_stats[3];
    int my_ostats[3];
    int opp_ostats[3];
    int opp_guess;
    int my_guess;
  };

  struct jlmhistret {
    int both, my, opp, num;
  };

  constexpr int pwill_beat(int x) { return (x + 1) % 3; }

  void jlm_history() {
    int besta, bestb, bestc, i, j, num;
    /* a is both history, b is my history, c is opponent history. */

    if (h_.num == history_len()) return;
    h_.num = num = history_len();
    h_.both = h_.my = h_.opp = besta = bestb = bestc = 0;
    for (i = num - 1; i > besta; --i) {
      for (j = 0; j < i && opp_history()[num - j] == opp_history()[i - j] &&
                  my_history()[num - j] == my_history()[i - j];
           ++j) {
      }
      if (j > besta) {
        besta = j;
        h_.both = i;
      }
      if (j > bestb) {
        bestb = j;
        h_.my = i;
      }
      if (j > bestc) {
        bestc = j;
        h_.opp = i;
      }
      if (opp_history()[num - j] != opp_history()[i - j]) {
        for (; j < i && my_history()[num - j] == my_history()[i - j]; ++j) {
        }
        if (j > bestb) {
          bestb = j;
          h_.my = i;
        }
      } else /* j >= i || my_history()[num-j] != my_history()[i - j] */ {
        for (; j < i && opp_history()[num - j] == opp_history()[i - j]; ++j) {
        }
        if (j > bestc) {
          bestc = j;
          h_.opp = i;
        }
      }
    }
  }

  long apply_jocaine(std::vector<jlmbot>& s) {
    int num = history_len();
    long b;
    int i, my_most, opp_most, h;
    int my_omost, opp_omost;
    int hy_omost, hpp_omost;
    int hy_most, hpp_most;

    if (0 == num) {
      for (h = 0; h < s.size(); h++) {
        for (i = 0; i < 3; ++i)
          s[h].my_stats[i] = s[h].opp_stats[i] = s[h].my_ostats[i] =
              s[h].opp_ostats[i] = 0;
        b = s[h].to_beat_fcn(*this);
        s[h].my_last = b & 0xFFFF;
        s[h].opp_last = b >> 16;
      }
      return random() % 3;
    }

    for (h = 0; h < s.size(); h++) {
      b = s[h].to_beat_fcn(*this);
      s[h].my_guess = b & 0xFFFF;
      s[h].opp_guess = b >> 16;

      s[h].my_stats[(3 + opp_history()[num] - s[h].my_last) % 3]++;
      s[h].opp_stats[(3 + opp_history()[num] - s[h].opp_last) % 3]++;
      s[h].my_ostats[(3 + my_history()[num] - s[h].opp_last) % 3]++;
      s[h].opp_ostats[(3 + my_history()[num] - s[h].my_last) % 3]++;

      s[h].my_last = s[h].my_guess;
      s[h].opp_last = s[h].opp_guess;
    }

    my_most = opp_most = my_omost = opp_omost = 0;
    hy_most = hpp_most = hy_omost = hpp_omost = 0;
    for (h = 0; h < s.size(); ++h)
      for (i = 0; i < 3; ++i) {
        if (s[h].my_stats[i] > s[hy_most].my_stats[my_most]) {
          my_most = i;
          hy_most = h;
        }
        if (s[h].opp_stats[i] > s[hpp_most].opp_stats[opp_most]) {
          opp_most = i;
          hpp_most = h;
        }
        if (s[h].my_ostats[i] > s[hy_omost].my_ostats[my_omost]) {
          my_omost = i;
          hy_omost = h;
        }
        if (s[h].opp_ostats[i] > s[hpp_omost].opp_ostats[opp_omost]) {
          opp_omost = i;
          hpp_omost = h;
        }
      }

    if (s[hpp_most].opp_stats[opp_most] >= s[hy_most].my_stats[my_most])
      b = pwill_beat((s[hpp_most].opp_guess + opp_most) % 3);
    else
      b = pwill_beat((s[hy_most].my_guess + my_most) % 3);

    if (s[hpp_omost].opp_ostats[opp_omost] >= s[hy_omost].my_ostats[my_omost])
      b |= pwill_beat((s[hpp_omost].my_guess + opp_omost) % 3) << 16;
    else
      b |= pwill_beat((s[hy_omost].opp_guess + my_omost) % 3) << 16;

    return b;
  }

  jlmhistret h_ = {};
  std::vector<jlmbot> t_;
  std::vector<jlmbot> s_;
};

}  // namespace roshambo_tournament

#endif  // PHASENBOTT_H
