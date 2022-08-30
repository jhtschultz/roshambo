#ifndef GREENBERG_H
#define GREENBERG_H

#include <cstdlib>
#include <vector>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Greenberg   Andrzej Nagorko (Poland)

 Greenberg is the winning bot from the second RoShamBo Programming
 Competition, held in 2000. Greenberg is not part of the original test
 suite file and was added subsequently in July, 2020. For more details
 about Greenberg, including the source of the source code, see:

 https://webdocs.cs.ualberta.ca/~darse/rsbpc.html
 http://www.mathpuzzle.com/older.htm
 http://www.mathpuzzle.com/greenberg.c
*/
class Greenberg : public RSBBot {
 public:
  Greenberg(int match_length) : RSBBot(match_length) {
    for (int i = 0; i < 4; ++i) {
      my_history_hash[i] = std::vector<short int>(match_length, 0);
      opp_history_hash[i] = std::vector<short int>(match_length, 0);
    }
    for (int i = 0; i < 24; ++i) {
      gear[i] = std::vector<int>(match_length, 0);
    }
  }

  int GetAction() override {
    int i;

    if (history_len() == 0) {
      init();
    } else {
      update_scores();
    }

    update_history_hash();
    make_predictions();

    for (i = 0; i < 6; i++) p_len[i] = find_best_prediction(lengths[i]);

    return p_len[find_max_index(s_len, 6)];
  }

 private:
  static constexpr int best_without[3] = {2, 0, 1};
  static constexpr int wins_with[3] = {1, 2, 0};
  static constexpr int score_table[3][3] = {{0, -1, 1}, {1, 0, -1}, {-1, 1, 0}};
  static constexpr int lengths[6] = {10, 20, 30, 40, 49, 0};

  int find_best_prediction(int len) {
    int i, j, k;
    int bs = -rsb_trials(), bp = 0;

    if (p_random_score > bs) {
      bs = p_random_score;
      bp = p_random;
    }

    for (i = 0; i < 3; i++) {
      for (j = 0; j < 24; j++) {
        for (k = 0; k < 4; k++) {
          if (p_full_score[history_len() % 50][j][k][i] -
                  (len ? p_full_score[(50 + history_len() - len) % 50][j][k][i]
                       : 0) >
              bs) {
            bs = p_full_score[history_len() % 50][j][k][i] -
                 (len ? p_full_score[(50 + history_len() - len) % 50][j][k][i]
                      : 0);
            bp = (p_full[j][k] + i) % 3;
          }
        }
        for (k = 0; k < 2; k++) {
          if (r_full_score[history_len() % 50][j][k][i] -
                  (len ? r_full_score[(50 + history_len() - len) % 50][j][k][i]
                       : 0) >
              bs) {
            bs = r_full_score[history_len() % 50][j][k][i] -
                 (len ? r_full_score[(50 + history_len() - len) % 50][j][k][i]
                      : 0);
            bp = (r_full[j][k] + i) % 3;
          }
        }
      }
      for (j = 0; j < 2; j++) {
        for (k = 0; k < 2; k++) {
          if (p_freq_score[history_len() % 50][j][k][i] -
                  (len ? p_freq_score[(50 + history_len() - len) % 50][j][k][i]
                       : 0) >
              bs) {
            bs = p_freq_score[history_len() % 50][j][k][i] -
                 (len ? p_freq_score[(50 + history_len() - len) % 50][j][k][i]
                      : 0);
            bp = (p_freq[j][k] + i) % 3;
          }
          if (r_freq_score[history_len() % 50][j][k][i] -
                  (len ? r_freq_score[(50 + history_len() - len) % 50][j][k][i]
                       : 0) >
              bs) {
            bs = r_freq_score[history_len() % 50][j][k][i] -
                 (len ? r_freq_score[(50 + history_len() - len) % 50][j][k][i]
                      : 0);
            bp = (r_freq[j][k] + i) % 3;
          }
        }
      }
    }

    return bp;
  }

  void update_scores() {
    int i, j, k;

    p_random_score += score_table[p_random][opp_last_move()];

    for (i = 0; i < 3; i++) {
      for (j = 0; j < 24; j++) {
        for (k = 0; k < 4; k++) {
          p_full_score[history_len() % 50][j][k][i] =
              p_full_score[(history_len() + 49) % 50][j][k][i] +
              score_table[(p_full[j][k] + i) % 3][opp_last_move()];
        }
        for (k = 0; k < 2; k++) {
          r_full_score[history_len() % 50][j][k][i] =
              r_full_score[(history_len() + 49) % 50][j][k][i] +
              score_table[(r_full[j][k] + i) % 3][opp_last_move()];
        }
      }
      for (j = 0; j < 2; j++) {
        for (k = 0; k < 2; k++) {
          p_freq_score[history_len() % 50][j][k][i] =
              p_freq_score[(history_len() + 49) % 50][j][k][i] +
              score_table[(p_freq[j][k] + i) % 3][opp_last_move()];
          r_freq_score[history_len() % 50][j][k][i] =
              r_freq_score[(history_len() + 49) % 50][j][k][i] +
              score_table[(r_freq[j][k] + i) % 3][opp_last_move()];
        }
      }
    }

    for (i = 0; i < 6; i++) s_len[i] += score_table[p_len[i]][opp_last_move()];
  }

  void init() {
    int i, j, k, l;

    for (i = 0; i < 24; ++i) {
      for (j = 0; j < 4; ++j) {
        p_full[i][j] = 0;
      }
      for (j = 0; j < 2; ++j) {
        r_full[i][j] = 0;
      }
    }
    for (i = 0; i < 2; ++i) {
      for (j = 0; j < 2; ++j) {
        p_freq[i][j] = 0;
        r_freq[i][j] = 0;
      }
    }
    for (i = 0; i < 2; ++i) {
      for (j = 0; j < 3; ++j) {
        freq[i][j] = 0;
      }
    }

    for (i = 0; i < 24; ++i) {
      for (j = 0; j < kCompetitionMatchLength; ++j) {
        gear[i][j] = 0;
      }
    }

    p_random_score = 0;
    p_random = biased_roshambo(0.3333, 0.3333);

    for (i = 0; i < 3; i++) {
      for (j = 0; j < 24; j++) {
        for (k = 0; k < 4; k++) {
          for (l = 0; l < 50; l++) p_full_score[l][j][k][i] = 0;
        }
        for (k = 0; k < 2; k++) {
          for (l = 0; l < 50; l++) r_full_score[l][j][k][i] = 0;
        }
      }
      for (j = 0; j < 2; j++) {
        for (k = 0; k < 2; k++) {
          for (l = 0; l < 50; l++) p_freq_score[l][j][k][i] = 0;
          for (l = 0; l < 50; l++) r_freq_score[l][j][k][i] = 0;
        }
      }
    }
    for (i = 0; i < 6; i++) {
      p_len[i] = 0;
      s_len[i] = 0;
    }
  }

  static int find_min_index(int* table, int length) {
    int i, min = 123456, min_i = -1;
    for (i = 0; i < length; i++)
      if (table[i] <= min) {
        min_i = i;
        min = table[i];
      }
    return min_i;
  }

  static int find_max_index(int* table, int length) {
    int i, max = -123456, max_i = -1;
    for (i = 0; i < length; i++)
      if (table[i] >= max) {
        max_i = i;
        max = table[i];
      }
    return max_i;
  }

  void make_predictions() {
    int i, j, k, l;
    int f[3][2][4][3];
    int t[3][2][4];

    std::vector<int> m_len[3] = {std::vector<int>(rsb_trials()),
                                 std::vector<int>(rsb_trials()),
                                 std::vector<int>(rsb_trials())};

    int value[2][3] = {{0, 0, 0}, {0, 0, 0}};

    int gear_freq[3];

    p_random = biased_roshambo(0.3333, 0.3333);

    for (i = 0; i < 24; i++) {
      gear[i][history_len()] = (3 + opp_last_move() - p_full[i][2]) % 3;
      if (history_len() > 1)
        gear[i][history_len()] += 3 * gear[i][history_len() - 1];
      gear[i][history_len()] = gear[i][history_len()] % 3;
    }

    if (history_len() == 0) {
      for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++) freq[i][j] = 0;
    } else {
      freq[0][my_last_move()]++;
      freq[1][opp_last_move()]++;

      for (i = 0; i < 2; i++) {
        value[i][0] = (1000 * (freq[i][2] - freq[i][1])) / history_len();
        value[i][1] = (1000 * (freq[i][0] - freq[i][2])) / history_len();
        value[i][2] = (1000 * (freq[i][1] - freq[i][0])) / history_len();
      }
    }

    for (i = 0; i < 2; i++) {
      p_freq[i][0] = wins_with[find_max_index(freq[i], 3)];
      p_freq[i][1] = wins_with[find_max_index(value[i], 3)];

      r_freq[i][0] = best_without[find_min_index(freq[i], 3)];
      r_freq[i][1] = best_without[find_min_index(value[i], 3)];
    }

    for (i = 0; i < 3; i++)
      for (j = 0; j < 2; j++)
        for (k = 0; k < 4; k++)
          for (l = 0; l < 3; l++) {
            f[i][j][k][l] = 0;
            t[i][j][k] = 0;
          }

    for (i = history_len() - 1; i > 0; i--) {
      m_len[0][i] = 4;
      for (j = 0; j < 4; j++)
        if (my_history_hash[j][i] != my_history_hash[j][history_len()]) {
          m_len[0][i] = j;
          break;
        }
      m_len[1][i] = 4;
      for (j = 0; j < 4; j++)
        if (opp_history_hash[j][i] != opp_history_hash[j][history_len()]) {
          m_len[1][i] = j;
          break;
        }
      m_len[2][i] = 4;
      for (j = 0; j < 4; j++)
        if (my_history_hash[j][i] != my_history_hash[j][history_len()] ||
            opp_history_hash[j][i] != opp_history_hash[j][history_len()]) {
          m_len[2][i] = j;
          break;
        }
    }

    for (i = history_len() - 1; i > 0; i--) {
      for (j = 0; j < 3; j++) {
        for (k = 0; k < m_len[j][i]; k++) {
          f[j][0][k][my_history()[i + 1]]++;
          t[j][0][k]++;
          f[j][1][k][opp_history()[i + 1]]++;
          t[j][1][k]++;

          if (t[j][0][k] == 1) {
            p_full[j * 8 + 0 * 4 + k][0] = wins_with[my_history()[i + 1]];
          }
          if (t[j][1][k] == 1) {
            p_full[j * 8 + 1 * 4 + k][0] = wins_with[opp_history()[i + 1]];
          }

          if (t[j][0][k] == 3) {
            p_full[j * 8 + 0 * 4 + k][1] =
                wins_with[find_max_index(f[j][0][k], 3)];
            r_full[j * 8 + 0 * 4 + k][0] =
                best_without[find_min_index(f[j][0][k], 3)];
          }

          if (t[j][1][k] == 3) {
            p_full[j * 8 + 1 * 4 + k][1] =
                wins_with[find_max_index(f[j][1][k], 3)];
            r_full[j * 8 + 1 * 4 + k][0] =
                best_without[find_min_index(f[j][1][k], 3)];
          }
        }
      }
    }

    for (j = 0; j < 3; j++) {
      for (k = 0; k < 4; k++) {
        p_full[j * 8 + 0 * 4 + k][2] = wins_with[find_max_index(f[j][0][k], 3)];
        r_full[j * 8 + 0 * 4 + k][1] =
            best_without[find_min_index(f[j][0][k], 3)];

        p_full[j * 8 + 1 * 4 + k][2] = wins_with[find_max_index(f[j][1][k], 3)];
        r_full[j * 8 + 1 * 4 + k][1] =
            best_without[find_min_index(f[j][1][k], 3)];
      }
    }

    for (j = 0; j < 24; j++) {
      for (i = 0; i < 3; i++) gear_freq[i] = 0;

      for (i = history_len() - 1; i > 0; i--)
        if (gear[j][i] == gear[j][history_len()]) gear_freq[gear[j][i + 1]]++;

      p_full[j][3] = (p_full[j][1] + find_max_index(gear_freq, 3)) % 3;
    }
  }

  void update_history_hash() {
    int i;

    if (history_len() == 0) {
      for (i = 0; i < 4; i++) {
        my_history_hash[i][0] = 0;
        opp_history_hash[i][0] = 0;
      }

      return;
    }

    my_history_hash[0][history_len()] = my_last_move();
    opp_history_hash[0][history_len()] = opp_last_move();

    for (i = 1; i < 4; i++) {
      my_history_hash[i][history_len()] =
          my_history_hash[i - 1][history_len() - 1] * 3 + my_last_move();
      opp_history_hash[i][history_len()] =
          opp_history_hash[i - 1][history_len() - 1] * 3 + opp_last_move();
    }
  }

  std::vector<short int> my_history_hash[4];
  std::vector<short int> opp_history_hash[4];

  int p_full[24][4];
  int p_freq[2][2];
  int p_random;

  std::vector<int> gear[24];

  int r_full[24][2];
  int r_freq[2][2];

  int p_full_score[50][24][4][3];
  int p_freq_score[50][2][2][3];
  int p_random_score;
  int r_full_score[50][24][2][3];
  int r_freq_score[50][2][2][3];

  int freq[2][3];

  int p_len[6];
  int s_len[6];
};

}  // namespace roshambo_tournament

#endif  // GREENBERG_H
