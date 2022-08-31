#ifndef RSB_BOT_H
#define RSB_BOT_H

#include <cassert>
#include <cstdlib>
#include <memory>

namespace roshambo_tournament {

class RSBBot {
 public:
  static constexpr int kCompetitionMatchLength = 1000;

  // Create a new RoShamBo bot for a repeated game with match_length trials per
  // game.
  RSBBot(int match_length) : match_length_(match_length) {
    my_history_ = std::make_unique<int[]>(match_length + 1);
    opp_history_ = std::make_unique<int[]>(match_length + 1);
    // Added for safety. Reset not needed currently, but that could change.
    Reset();
  }

  virtual ~RSBBot() = default;

  // Reset the current match.
  void Reset() {
    my_history_[0] = 0;
    opp_history_[0] = 0;
  }

  // Records a completed trial.
  void RecordTrial(int my_action, int opp_action) {
    assert(my_history_[0] < rsb_trials());
    ++my_history_[0];
    my_history_[my_history_[0]] = my_action;
    ++opp_history_[0];
    opp_history_[opp_history_[0]] = opp_action;
  }

  // Returns an action for the current trial.
  virtual int GetAction() = 0;

  // Returns number of throws completed in current match.
  int CurrentMatchLength() const { return my_history_[0]; }

  // History is stored as history_length, oldest_action, next_action, ...
  // Not using std::vector so that we exactly match this competition format.
  const int* MyHistory() const { return my_history_.get(); }
  const int* OppHistory() const { return opp_history_.get(); }

 protected:
  int rsb_trials() const { return match_length_; }
  int history_len() const { return my_history_[0]; }
  const int* my_history() const { return my_history_.get(); }
  const int* opp_history() const { return opp_history_.get(); }
  // Competition code had a common pattern of history[history[0]].
  int my_last_move() const { return my_history_[my_history_[0]]; }
  int opp_last_move() const { return opp_history_[opp_history_[0]]; }

  // Helper functions and constants from the original RoShamBo competition. They
  // follow the original code to reproduce original behaviour as closely as
  // possible, rather than whatever current best practices might be.
  static constexpr int kRock = 0;
  static constexpr int kPaper = 1;
  static constexpr int kScissors = 2;

  /* 2^31, ratio range is 0 <= r < 1 */
  static constexpr double kMaxRandom = 2147483648.0;

  /* Flip an unfair coin (bit) with given probability of getting a 1. */
  static bool flip_biased_coin(double prob) {
    return random() / kMaxRandom < prob;
  }

  /* RoShamBo with given probabilities of rock, paper, or scissors. */
  static int biased_roshambo(double prob_rock, double prob_paper) {
    double spinner = random() / kMaxRandom;
    if (spinner < prob_rock)
      return kRock;
    else if (spinner < prob_rock + prob_paper)
      return kPaper;
    else
      return kScissors;
  }

 private:
  int match_length_;
  std::unique_ptr<int[]> my_history_;
  std::unique_ptr<int[]> opp_history_;
};

}  // namespace roshambo_tournament

#endif  // RSB_BOT_H
