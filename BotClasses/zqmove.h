#ifndef ZQMOVE_H
#define ZQMOVE_H

#include <algorithm>
#include <cstdlib>
#include <memory>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  ZQ Bot (22)   Neil Burch (Can)  */
class ZQMove : public RSBBot {
 public:
  static constexpr int ZQ_MAXR = 2147483645;
  static constexpr int ZQ_TIE = 0;
  static constexpr int ZQ_WIN = 1;
  static constexpr int ZQ_LOSS = 2;
  static constexpr int ZQ_MAX_NODES = 65536;
  static constexpr int ZQ_MAX_LOSS = 15;
  static constexpr int zq_patt_length = 9;

  ZQMove(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int start, i, j, counts[3], move, closs;
    zq_node *node;

    move = 0;
    closs = 0; /* -db */
    if (!history_len()) {
      losestreak_ = 0;
      closs = 0;
      zq_init();
    }

    if (zq_calc_result(my_last_move(), opp_last_move()) == ZQ_LOSS) {
      if (losestreak_) {
        closs++;
        if (closs == ZQ_MAX_LOSS) {
          losestreak_ = 0;
          closs = 0;
          zq_init();
        }
      } else {
        losestreak_ = 1;
        closs = 1;
      }
    } else
      losestreak_ = 0;

    /* update tree */
    zq_walk_history();

    for (i = 0; i < 3; i++) counts[i] = 0;
    start = history_len() - zq_patt_length + 1;
    if (start < 1) start = 1;
    for (; start <= history_len(); start++) {
      node = &zq_root_node_;
      for (i = start; i <= history_len(); i++) {
        if (!node->children) break;
        node =
            &(node->children[ZQ_MOVE_PAIR(my_history()[i], opp_history()[i])]);
      }
      if (i > history_len())
        if (node->children)
          for (i = 0; i < 3; i++)   /* opponent choice */
            for (j = 0; j < 3; j++) /* my choice */
              counts[i] +=
                  node->children[ZQ_MOVE_PAIR(j, i)].count * node->count;
    }

    if (counts[1] > counts[0])
      j = counts[1];
    else
      j = counts[0];
    if (counts[2] > j) j = counts[2];

    i = 0;
    if (counts[0] == j) i++;
    if (counts[1] == j) i++;
    if (counts[2] == j) i++;

    if (i == 3)
      move = zq_random_move();
    else if ((i == 1) || (random() & 1)) {
      /* only one choice, or first choice of two */
      for (i = 0; i < 3; i++)
        if (counts[i] == j) {
          move = i;
          break;
        }
    } else {
      for (i = 2; i >= 0; i--)
        if (counts[i] == j) {
          move = i;
          break;
        }
    }

    return (move + 1) % 3;
  }

 private:
  struct zq_node {
    std::unique_ptr<zq_node[]> children = nullptr;
    int count = 0;
  };

  static int ZQ_MOVE_PAIR(int me, int them) { return me * 3 + them; }

  static int zq_calc_result(int me, int them) {
    int t;

    t = (me - them) % 3;
    if (t < 0) t += 3;
    return t;
  }

  bool zq_expand_node(zq_node *node) {
    if (zq_num_nodeblocks_ >= ZQ_MAX_NODES) return false;
    node->children = std::make_unique<zq_node[]>(9);
    ++zq_num_nodeblocks_;
    return true;
  }

  static int zq_random_move() {
    /* random%3 does not produce an equal distribution */
    int t;

    do {
      t = random();
    } while (t > ZQ_MAXR);
    return t % 3;
  }

  void zq_init() {
    int i;

    zq_root_node_.children.reset(nullptr);
    zq_num_nodeblocks_ = 0;

    /* ensure at least two moves can be remembered */
    zq_expand_node(&zq_root_node_);
    for (i = 0; i < 9; i++) zq_expand_node(&zq_root_node_.children[i]);
  }

  void zq_walk_history() {
    int start, i;
    zq_node *node;

    if (!history_len()) return;

    /* walk the tree for last zq_patt_length moves, last 6, ..., last move */
    start = history_len() - zq_patt_length + 1;
    if (start < 1) start = 1;
    for (; start <= history_len(); start++) {
      node = &zq_root_node_;
      for (i = start; i <= history_len(); i++) {
        if (!node->children)
          if (!zq_expand_node(node)) break;
        node =
            &(node->children[ZQ_MOVE_PAIR(my_history()[i], opp_history()[i])]);
      }
      if (i > history_len()) node->count++;
    }
  }

  char losestreak_;
  zq_node zq_root_node_;
  int zq_num_nodeblocks_;
};

}  // namespace roshambo_tournament

#endif  // ZQMOVE_H
