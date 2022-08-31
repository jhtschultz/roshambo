#ifndef MULTIBOT_H
#define MULTIBOT_H

#include <cstdlib>
#include <vector>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Multi-strategy (38)   Mark James (Can) */
class MultiBot : public RSBBot {
 public:
  static constexpr int strategy_count = 7;
  static constexpr int AVGLEN = 50;

  MultiBot(int match_length) : RSBBot(match_length) {}

  int GetAction() override;

  int random_md5() {
    if (FirstTrial()) {
      Roshambo_MD5Init();
    }
    return Roshambo_MD5Random() % 3;
  }

  int mrockbot() {
    /* "Good ole rock.  Nuthin' beats rock." */
    return (kRock);
  }

  int mpaperbot() { return (kPaper); }

  int mscissorsbot() { return (kScissors); }

  int beatcopybot() { return ((my_last_move() + 2) % 3); }

  int beatswitchbot() {
    /* assume opponent never repeats the previous pick */
    if (opp_last_move() == kRock) {
      return (kScissors);
    } else if (opp_last_move() == kPaper) {
      return (kRock);
    } else
      return (kPaper);
  }

  int beatfreqbot() {
    /* play whatever will beat the freqbot choice */

    int i, rcount, pcount, scount;

    rcount = 0;
    pcount = 0;
    scount = 0;
    for (i = 1; i <= history_len(); i++) {
      if (my_history()[i] == kRock) {
        rcount++;
      } else if (my_history()[i] == kPaper) {
        pcount++;
      } else /* my_history()[i] == kScissors */ {
        scount++;
      }
    }
    if ((rcount > pcount) && (rcount > scount)) {
      return (kScissors);
    } else if (pcount > scount) {
      return (kRock);
    } else {
      return (kPaper);
    }
  }

 private:
  struct RollingAverage {
    RollingAverage(int length)
        : total(0.0), count(0), size(length), next(0), data(length, 0.0) {}

    float total;
    int count;
    int size;
    int next;
    std::vector<float> data;
  };

  struct Strategy {
    Strategy(int (*function)(MultiBot&), int length)
        : success(length), function(function), lastmove(0), used(0) {}

    RollingAverage success;
    int (*function)(MultiBot&);
    int lastmove;
    int used;
  };

  bool FirstTrial() { return history_len() == 0; }

  int Strategy_Move(Strategy* stgy) {
    int move = (stgy->function)(*this);
    stgy->lastmove = move;
    return move;
  }

  void Strategy_Use(Strategy* stgy) { stgy->used++; }

  int Strategy_Used(Strategy* stgy) { return stgy->used; }

  float Score(int round, int move) {
    int p1 = move;
    int p2 = opp_history()[round];

    if (p1 == p2) {
      return 0.5f;
    } else if ((p1 - p2 == 1) || (p1 - p2 == -2)) {
      return 1.0f;
    } else if ((p2 - p1 == 1) || (p2 - p1 == -2)) {
      return 0.0f;
    }
    return 0.0f;
  }

  float RollingAverage_Add(RollingAverage* avg, float element) {
    avg->total -= avg->data[avg->next];
    avg->data[avg->next] = element;
    avg->next = (avg->next + 1) % avg->size;
    avg->total += element;
    avg->count++;
    if (avg->count > avg->size) {
      avg->count = avg->size;
    }
    return RollingAverage_Average(avg);
  }

  float RollingAverage_Average(RollingAverage* avg) {
    if (avg->count == 0) {
      return 0.0f;
    } else {
      /*
        printf("Total: %f Count: %d Avg: %f\n",
        avg->total,
        avg->count,
        avg->total / avg->count); */

      return avg->total / avg->count;
    }
  }

  /*
   * MD5 transform algorithm, taken from code written by Colin Plumb,
   * and put into the public domain
   */

  /* The four core functions */

#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

  /* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) \
  (w += f(x, y, z) + data, w = w << s | w >> (32 - s), w += x)

  /*
   * The core of the MD5 algorithm, this alters an existing MD5 hash to
   * reflect the addition of 16 longwords of new data.  MD5Update blocks
   * the data and converts bytes into longwords for this routine.
   */
  static void MD5Transform(unsigned int buf[4], unsigned int const in[16]) {
    unsigned int a, b, c, d;

    a = buf[0];
    b = buf[1];
    c = buf[2];
    d = buf[3];

    MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
    MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
    MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
    MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
    MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
    MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
    MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
    MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
    MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
    MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
    MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

    MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
    MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
    MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
    MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
    MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
    MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
    MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
    MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
    MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

    MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
    MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
    MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
    MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
    MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
    MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

    MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
    MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
    MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
    MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
    MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
    MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
    MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
    MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
    MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
    MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
    MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
  }

#undef F1
#undef F2
#undef F3
#undef F4
#undef MD5STEP

  void Roshambo_MD5Init() {
    int i;
    for (i = 0; i < 4; i++) {
      MD5Buf[i] = MD5BufInit[i];
    }

    for (i = 0; i < 16; i++) {
      MD5In[i] = MD5Buf[i % 4] + MD5Buf[i / 4];
    }
  }

  unsigned int Roshambo_MD5Random() {
    int i;
    MD5Transform(MD5Buf, MD5In);
    for (i = 0; i < 16; i++) {
      MD5In[i] = MD5Buf[i % 4] + MD5Buf[i / 4];
    }

    return MD5Buf[0];
  }

  std::vector<Strategy> strategies;
  unsigned int MD5Buf[4];
  unsigned int MD5In[16];

  unsigned int MD5BufInit[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
};

}  // namespace roshambo_tournament

#endif  // MULTIBOT_H
