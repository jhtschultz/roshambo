#ifndef BOOM_H
#define BOOM_H

#include <cmath>
#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Boom (10)   Jack van Rijswijk (Net)  */
class Boom : public RSBBot {
 public:
  Boom(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int boom_history = 27;
    float lambda = 0.95;

    int boom_secondary_stats[28][4][4][3];

    float bail_min, bail_max, bail;
    float bail_l_min, bail_l_max, bail_l_diff;

    int turn, action;
    int i, j;
    int opp_earlier, my_earlier, opp_last, my_last;
    float best, pred_r, pred_p, pred_s;

    int filter_opp, filter_me, filter_lag;

    pred_r = 0;
    pred_p = 0;
    pred_s = 0;
    filter_opp = 0;
    filter_me = 0;
    filter_lag = -1;
    turn = history_len();

    bail_l_min = sqrt((1 - lambda) / 3);
    bail_l_max = sqrt(2 * (1 - lambda));
    bail_l_diff = bail_l_min - bail_l_max;

    if (turn == 0) { /* initialize arrays */
      int k, l;
      for (i = 0; i < boom_history; i++)
        for (j = 0; j < 4; j++)
          for (k = 0; k < 4; k++)
            for (l = 0; l < 3; l++) boom_stats_[i][j][k][l] = 0;
      boom_overall_ = 0;
      boom_gear_ = 0;
      for (i = 0; i < 3; i++) boom_gear_success_[i] = 0;
      boom_recent_success_ = 0;
    } else { /* update statistics */

      opp_last = opp_history()[turn];
      my_last = my_history()[turn];

      for (i = 0; i < boom_history; i++) {
        if (turn - i - 1 > 0) {
          opp_earlier = opp_history()[turn - i - 1];
          my_earlier = my_history()[turn - i - 1];

          boom_stats_[i][opp_earlier][my_earlier][opp_last]++;
          boom_stats_[i][3][my_earlier][opp_last]++;
          boom_stats_[i][opp_earlier][3][opp_last]++;
          boom_stats_[i][3][3][opp_last]++;
        }
      }

      for (i = 0; i < 3; i++) boom_gear_success_[i] *= lambda;
      boom_recent_success_ *= lambda;

      j = boom_rps_result(my_last, opp_last); /* win/tie/loss previous turn */
      if (j == -1) {
        boom_overall_--;
        boom_recent_success_ -= 1 - lambda;
        boom_gear_success_[boom_gear_] -= 1 - lambda;
        boom_gear_success_[boom_rotate(boom_gear_, -1)] += 1 - lambda;
      } else if (j == 1) {
        boom_overall_++;
        boom_recent_success_ += 1 - lambda;
        boom_gear_success_[boom_gear_] += 1 - lambda;
        boom_gear_success_[boom_rotate(boom_gear_, +1)] -= 1 - lambda;
      } else {
        boom_gear_success_[boom_rotate(boom_gear_, +1)] += 1 - lambda;
        boom_gear_success_[boom_rotate(boom_gear_, -1)] -= 1 - lambda;
      }
    }

    /* check current context */
    best = 0;

    for (i = 0; i < boom_history; i++)
      // Note: fixed a buffer overflow. i == turn -> xyz_early == history_len.
      if (i < turn) {
        int r, p, s, t;
        float w;

        opp_earlier = opp_history()[turn - i];
        my_earlier = my_history()[turn - i];

        r = boom_stats_[i][opp_earlier][my_earlier][0];
        p = boom_stats_[i][opp_earlier][my_earlier][1];
        s = boom_stats_[i][opp_earlier][my_earlier][2];
        w = boom_getrelevance(r, p, s);
        if (w > best) {
          best = w;
          t = r + p + s;
          pred_r = (float)r / t;
          pred_p = (float)p / t;
          pred_s = (float)s / t;
          filter_opp = opp_earlier;
          filter_me = my_earlier;
          filter_lag = i;
        }

        r = boom_stats_[i][3][my_earlier][0];
        p = boom_stats_[i][3][my_earlier][1];
        s = boom_stats_[i][3][my_earlier][2];
        w = boom_getrelevance(r, p, s);
        if (w > best) {
          best = w;
          t = r + p + s;
          pred_r = (float)r / t;
          pred_p = (float)p / t;
          pred_s = (float)s / t;
          filter_opp = 3;
          filter_me = my_earlier;
          filter_lag = i;
        }

        r = boom_stats_[i][opp_earlier][3][0];
        p = boom_stats_[i][opp_earlier][3][1];
        s = boom_stats_[i][opp_earlier][3][2];
        w = boom_getrelevance(r, p, s);
        if (w > best) {
          best = w;
          t = r + p + s;
          pred_r = (float)r / t;
          pred_p = (float)p / t;
          pred_s = (float)s / t;
          filter_opp = opp_earlier;
          filter_me = 3;
          filter_lag = i;
        }

        r = boom_stats_[i][3][3][0];
        p = boom_stats_[i][3][3][1];
        s = boom_stats_[i][3][3][2];
        w = boom_getrelevance(r, p, s);
        if (w > best) {
          best = w;
          t = r + p + s;
          pred_r = (float)r / t;
          pred_p = (float)p / t;
          pred_s = (float)s / t;
          filter_opp = 3;
          filter_me = 3;
          filter_lag = i;
        }
      }

    /* filter statistics, get second-order stats */
    /*    only if we're less than 95% sure so far */

    if ((best < 0.95) && (filter_lag >= 0)) {
      int k, l, r, p, s, t;
      float w;

      /* reset 2nd order stats */
      for (i = 0; i < boom_history; i++)
        for (j = 0; j < 4; j++)
          for (k = 0; k < 4; k++)
            for (l = 0; l < 3; l++) boom_secondary_stats[i][j][k][l] = 0;

      /* get 2nd order stats */
      for (i = filter_lag + 2; i <= turn; i++) {
        opp_earlier = opp_history()[i - filter_lag - 1];
        my_earlier = my_history()[i - filter_lag - 1];
        if (((filter_opp == 3) || (filter_opp == opp_earlier)) &&
            ((filter_me == 3) || (filter_me == my_earlier))) {
          opp_last = opp_history()[i];
          for (j = 0; j < boom_history; j++) {
            if (i - j - 1 > 0) {
              opp_earlier = opp_history()[i - j - 1];
              my_earlier = my_history()[i - j - 1];
              boom_secondary_stats[j][opp_earlier][my_earlier][opp_last]++;
              boom_secondary_stats[j][3][my_earlier][opp_last]++;
              boom_secondary_stats[j][opp_earlier][3][opp_last]++;
              boom_secondary_stats[j][3][3][opp_last]++;
            }
          }
        }
      }

      /* any better information in there? */
      for (i = 0; i < boom_history; i++)
        if (i < turn) {
          opp_earlier = opp_history()[turn - i];
          my_earlier = my_history()[turn - i];

          r = boom_secondary_stats[i][opp_earlier][my_earlier][0];
          p = boom_secondary_stats[i][opp_earlier][my_earlier][1];
          s = boom_secondary_stats[i][opp_earlier][my_earlier][2];
          w = boom_getrelevance(r, p, s);
          if (w > best) {
            best = w;
            t = r + p + s;
            pred_r = (float)r / t;
            pred_p = (float)p / t;
            pred_s = (float)s / t;
          }

          r = boom_secondary_stats[i][3][my_earlier][0];
          p = boom_secondary_stats[i][3][my_earlier][1];
          s = boom_secondary_stats[i][3][my_earlier][2];
          w = boom_getrelevance(r, p, s);
          if (w > best) {
            best = w;
            t = r + p + s;
            pred_r = (float)r / t;
            pred_p = (float)p / t;
            pred_s = (float)s / t;
          }

          r = boom_secondary_stats[i][opp_earlier][3][0];
          p = boom_secondary_stats[i][opp_earlier][3][1];
          s = boom_secondary_stats[i][opp_earlier][3][2];
          w = boom_getrelevance(r, p, s);
          if (w > best) {
            best = w;
            t = r + p + s;
            pred_r = (float)r / t;
            pred_p = (float)p / t;
            pred_s = (float)s / t;
          }
        }
    }

    /* got the predicted probabilities of r-p-s -- determine suggested action */
    best = pred_s - pred_p;
    action = kRock;
    if ((pred_r - pred_s) > best) {
      best = pred_r - pred_s;
      action = kPaper;
    }
    if ((pred_p - pred_r) > best) action = kScissors;

    /* modify the action according to the gears */
    best = boom_gear_success_[0];
    boom_gear_ = 0;
    if (boom_gear_success_[1] > best) {
      best = boom_gear_success_[1];
      boom_gear_ = 1;
    }
    if (boom_gear_success_[2] > best) {
      best = boom_gear_success_[2];
      boom_gear_ = 2;
    }
    action = (action + boom_gear_) % 3;

    /* ignore the action altogether if we're losing */
    /* global bailout */
    bail_min = (float)sqrt(turn) / sqrt(3.0);
    bail_max = (float)sqrt(turn) * sqrt(2.0);
    if (bail_min < bail_max)
      bail = (float)(bail_min + boom_overall_) / (bail_min - bail_max);
    else
      bail = 0;

    /* local bailout */
    if ((boom_recent_success_ + bail_l_min) / bail_l_diff > bail)
      bail = (boom_recent_success_ + bail_l_min) / bail_l_diff;

    if (bail < 0) bail = 0;
    if (bail > 1) bail = 1;

    /* final decision: going random this turn? */
    if (flip_biased_coin(bail))
      action = biased_roshambo((float)1 / 3, (float)1 / 3);

    return (action);
  }

 private:
  static float boom_getrelevance(int r, int p, int s) {
    float best;

    best = s - p;
    if (r - s > best) best = r - s;
    if (p - r > best) best = p - r;

    return (best / (r + p + s + 5));
  }

  static int boom_rotate(int rps, int increment) {
    rps = (rps + increment) % 3;
    if (rps < 0) rps += 3;
    return (rps);
  }

  static int boom_rps_result(int action1, int action2) {
    if (action1 == action2) return (0);
    if (boom_rotate(action1, 1) == action2) return (-1);
    return (1);
  }

  int boom_stats_[28][4][4][3];
  int boom_overall_;
  int boom_gear_;
  float boom_gear_success_[3];
  float boom_recent_success_;
};

}  // namespace roshambo_tournament

#endif  // BOOM_H
