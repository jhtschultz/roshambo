#ifndef RUSSROCKER4_H
#define RUSSROCKER4_H

#include <algorithm>
#include <cstdlib>
#include <cstring>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  RussRocker4 (4)   Russ Williams (USA)

   > I also welcome more feedback from the participants,

 Ok, here's some more feedback & personal info for you.  Feel free to include
 any of it at your site if it seems of interest.

 You summed up the basic idea of my AI pretty well in your Part 1 report.  I
 basically made a Markov model of the other player's actions, given the last
 3 moves of both players and basing the probabilities on the entire match
 history.  I then assumed they would simply pick the most likely move.  I
 also used the last 2 moves if the last 3 moves gave a tie for most likely
 guess, and if that still tied, use the last move, and so on.  Experimenting
 showed that using this tie breaking seemed to only be useful early on, so
 after a while ties for most likely opponent choice were broken by choosing
 randomly.

 I also intentionally chose to use the large arrays to avoid having to scan
 the entire history array each turn, since I wasn't sure how much of an issue
 execution speed would be.  The cost of that was that there was no simple or
 obvious way to give more emphasis to more recent games, which I would have
 liked to have done.

 I'd misunderstood and thought that reverting to random behavior even as a
 "bailout" measure was considered unsporting, else I might have added such a
 feature which would have (as you observed) saved me getting so trounced by
 the rank 1 & 2 programs.  Or did you have some other sort of bailout measure
 in mind?  I could imagine another potentially useful (or at least amusing)
 bailout measure would be "if I'm losing hideously, then start doing the
 opposite of whatever my algorithm says I should do."

 I fiddled off & on with my program for about 5 days.  It went through quite
 a few iterations, and I played many long tournaments with variations of
 itself and lots of intentionally weak players to tweak it.

 I also found that some versions seemed much stronger at short matches (e.g.
 100 games) and weaker at long matches (e.g. 10000 games), and vice-versa.
 The reasons were not always apparent.

 In real life I am a game programmer, which I got into after completing a MS
 in CS at UT Austin.  I worked on 1830 (from Avalon Hill) and Master of Orion
 2 (from Microprose), doing AI for both.  I plan to work on AI for Go one of
 these days.
*/
class RussRocker4 : public RSBBot {
 public:
  RussRocker4(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    /* by Russ Williams (e-mail: russrocker at hotmail dot com */
    const int n_moves_for_3 = 825;
    const int n_moves_for_2 = 11;
    const int n_moves_for_1 = 6;

    int max_index, max_value;

    int i, j, n;

    int n_moves = history_len();
    int their_last = -1;

    int temp[3];
    int n_votes[3] = {1, 1, 1};

    if (n_moves == 0) {
      memset(moves0_, 0, sizeof moves0_);
      memset(moves1_, 0, sizeof moves1_);
      memset(moves2_, 0, sizeof moves2_);
      memset(moves3_, 0, sizeof moves3_);
    } else {
      their_last = opp_history()[n_moves];
    }

    switch (n_moves) {
      default:
        ++moves3_[my_history()[n_moves - 3]][opp_history()[n_moves - 3]]
                 [my_history()[n_moves - 2]][opp_history()[n_moves - 2]]
                 [my_history()[n_moves - 1]][opp_history()[n_moves - 1]]
                 [their_last];
        [[fallthrough]];
      case 3:
        ++moves2_[my_history()[n_moves - 2]][opp_history()[n_moves - 2]]
                 [my_history()[n_moves - 1]][opp_history()[n_moves - 1]]
                 [their_last];
        [[fallthrough]];
      case 2:
        ++moves1_[my_history()[n_moves - 1]][opp_history()[n_moves - 1]]
                 [their_last];
        [[fallthrough]];
      case 1:
        ++moves0_[their_last];
        [[fallthrough]];
      case 0:
        break;
    }

    do {
      if (3 <= n_moves) {
        for (i = 0; i < 3; ++i) {
          temp[i] =
              moves3_[my_history()[n_moves - 2]][opp_history()[n_moves - 2]]
                     [my_history()[n_moves - 1]][opp_history()[n_moves - 1]]
                     [my_history()[n_moves]][their_last][i];
        }
        max_index = russrock_max(temp, 3);
        max_value = temp[max_index];
        if (0 < max_value) {
          n = 0;
          for (i = 0; i < 3; ++i) {
            if (temp[i] == max_value) {
              n_votes[i] += 10000;
              ++n;
            }
          }
          if (n == 1 || n_moves_for_3 <= n_moves) break;
        }

        for (i = 0; i < 3; ++i) {
          temp[i] = 0;
          for (j = 0; j < 3; ++j) {
            temp[i] +=
                moves3_[j][opp_history()[n_moves - 2]]
                       [my_history()[n_moves - 1]][opp_history()[n_moves - 1]]
                       [my_history()[n_moves]][their_last][i] +
                moves3_[my_history()[n_moves - 2]][j][my_history()[n_moves - 1]]
                       [opp_history()[n_moves - 1]][my_history()[n_moves]]
                       [their_last][i];
          }
        }
        max_index = russrock_max(temp, 3);
        max_value = temp[max_index];
        if (0 < max_value) {
          n = 0;
          for (i = 0; i < 3; ++i) {
            if (temp[i] == max_value) {
              n_votes[i] += 5000;
              ++n;
            }
          }
        }
      }

      if (2 <= n_moves) {
        for (i = 0; i < 3; ++i) {
          temp[i] =
              moves2_[my_history()[n_moves - 1]][opp_history()[n_moves - 1]]
                     [my_history()[n_moves]][their_last][i];
        }
        max_index = russrock_max(temp, 3);
        max_value = temp[max_index];
        if (0 < max_value) {
          n = 0;
          for (i = 0; i < 3; ++i) {
            if (temp[i] == max_value) {
              n_votes[i] += 1000;
              ++n;
            }
          }
          if (n_moves_for_2 <= n_moves) break;
        }

        for (i = 0; i < 3; ++i) {
          temp[i] = 0;
          for (j = 0; j < 3; ++j) {
            temp[i] += moves2_[j][opp_history()[n_moves - 1]]
                              [my_history()[n_moves]][their_last][i] +
                       moves2_[my_history()[n_moves - 1]][j]
                              [my_history()[n_moves]][their_last][i];
          }
        }
        max_index = russrock_max(temp, 3);
        max_value = temp[max_index];
        if (0 < max_value) {
          n = 0;
          for (i = 0; i < 3; ++i) {
            if (temp[i] == max_value) {
              n_votes[i] += 500;
              ++n;
            }
          }
        }
      }

      if (1 <= n_moves) {
        for (i = 0; i < 3; ++i) {
          temp[i] = moves1_[my_history()[n_moves]][their_last][i];
        }
        max_index = russrock_max(temp, 3);
        max_value = temp[max_index];
        if (0 < max_value) {
          n = 0;
          for (i = 0; i < 3; ++i) {
            if (temp[i] == max_value) {
              n_votes[i] += 100;
              ++n;
            }
          }
          if (n_moves_for_1 <= n_moves) break;
        }

        for (i = 0; i < 3; ++i) {
          temp[i] = 0;
          for (j = 0; j < 3; ++j) {
            temp[i] += moves1_[j][their_last][i] +
                       moves1_[my_history()[n_moves]][j][i];
          }
        }
        max_index = russrock_max(temp, 3);
        max_value = temp[max_index];
        if (0 < max_value) {
          n = 0;
          for (i = 0; i < 3; ++i) {
            if (temp[i] == max_value) {
              n_votes[i] += 50;
              ++n;
            }
          }
        }
      }

      {
        for (i = 0; i < 3; ++i) {
          temp[i] = moves0_[i];
        }
        max_index = russrock_max(temp, 3);
        max_value = temp[max_index];
        if (0 < max_value) {
          n = 0;
          for (i = 0; i < 3; ++i) {
            if (temp[i] == max_value) {
              n_votes[i] += 10;
              ++n;
            }
          }
        }
      }
    } while (0);

    max_index = russrock_max(n_votes, 3);
    for (i = 0; i < 3; ++i) {
      if (n_votes[i] < n_votes[max_index]) {
        n_votes[i] = 0;
      }
    }

    return (1 +
            biased_roshambo(
                n_votes[0] / (double)(n_votes[0] + n_votes[1] + n_votes[2]),
                n_votes[1] / (double)(n_votes[0] + n_votes[1] + n_votes[2]))) %
           3;
  }

 private:
  int russrock_max(int *a, int n) {
    int i;
    int best_index = 0;
    int max_so_far = a[0];
    for (i = 1; i < n; ++i) {
      if (max_so_far < a[i]) {
        max_so_far = a[i];
        best_index = i;
      }
    }
    return best_index;
  }

  int moves0_[3];
  int moves1_[3][3][3];
  int moves2_[3][3][3][3][3];
  int moves3_[3][3][3][3][3][3][3];
};

}  // namespace roshambo_tournament

#endif  // RUSSROCKER4_H
