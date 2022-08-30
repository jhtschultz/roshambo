#ifndef INOCENCIO_H
#define INOCENCIO_H

#include <cstdlib>

#include "rsb_bot.h"

namespace roshambo_tournament {

/*  Entrant:  Inocencio (44)   Rafael Morales (Mex)

 Darse,

 Thank you very much for your email about the results from the
 competition. They are extremely interesting and revealing. Certainly,
 I never thought the competition were a waste of time, but a clearly
 underestimate the richness of possible (good) solutions available.
 The range of techniques discussed in your email gives me a clearer
 idea of the complexity of opponent modelling. (My own research is on
 learner modelling, but since I am interested in learning to play
 games as test-bed application I should know better about opponent
 modelling).

 Inocencio's results were pretty bad, as it deserved.  No surprise
 after reading the descriptions of the strongest players.  I am
 looking forward to hearing about the next competition.  If I decided
 to participate, I shall do a much better job.

 Congratulations.  You have done a very good job.

 ==
 Rafael Morales (PhD student)                | 80 South Bridge
 School of Artificial Intelligence           | Edinburgh
 Division of Informatics                     | EH1 1HN
 University of Edinburgh                     | Scotland, UK

*/
class Inocencio : public RSBBot {
 public:
  Inocencio(int match_length) : RSBBot(match_length) {}

  int GetAction() override {
    int opp_last;
    int opp_fgt;
    int n, total;

    float rstat, pstat, sstat;

    float pat[27][3];
    float mypat[27][3];
    int patCount[27];
    int mypatCount[27];
    float probs[3] = {0.0, 0.0, 0.0};
    float my_probs[3] = {0.0, 0.0, 0.0};
    int i, j, b, pi, x;

    pi = 0; /* -db */
    n = history_len();

    if (n == 0) {
      rcount = 0;
      pcount = 0;
      scount = 0;
    } else {
      opp_last = opp_history()[n];
      if (opp_last == kRock) {
        rcount++;
      } else if (opp_last == kPaper) {
        pcount++;
      } else /* opp_last == kScissors */ {
        scount++;
      }

      if (n > 20) {
        opp_fgt = opp_history()[n - 20];
        if (opp_fgt == kRock) {
          rcount--;
        } else if (opp_fgt == kPaper) {
          pcount--;
        } else /* opp_fgt == kScissors */ {
          scount--;
        }
      }
    }
    total = rcount + pcount + scount;

    rstat = (rcount + 1.0) / (total + 3.0);
    pstat = (pcount + 1.0) / (total + 3.0);
    sstat = (scount + 1.0) / (total + 3.0);

    if (n < 20) {
      return (random() % 3);
    }

    if (rstat > 0.45) {
      return (kPaper);
    } else if (pstat > 0.45) {
      return (kScissors);
    } else if (sstat > 0.45) {
      return (kRock);
    }

    for (i = 0; i < 27; i++) {
      patCount[i] = 0;
      for (j = 0; j < 3; j++) pat[i][j] = 0.0;
    }

    for (i = 1; i < n - 1; i++) {
      pi = 0;
      b = 1;
      for (j = 0; j < 3; j++) {
        x = opp_history()[i + j];
        pi += b * x;
        b *= 3;
      }
      if (i < n - 2) {
        pat[pi][opp_history()[i + 3]] += 1;
        patCount[pi]++;
      }
    }
    for (i = 0; i < 27; i++)
      for (j = 0; j < 3; j++)
        pat[i][j] = (pat[i][j] + 1.0) / (patCount[i] + 3.0);

    for (j = 0; j < 3; j++) probs[j] += pat[pi][j];

    for (j = 0; j < 3; j++)
      if (pat[pi][j] > 0.45) {
        if (j == kRock)
          return kPaper;
        else if (j == kPaper)
          return kScissors;
        else
          return kRock;
      }

    for (i = 0; i < 27; i++) {
      mypatCount[i] = 0;
      for (j = 0; j < 3; j++) mypat[i][j] = 0.0;
    }

    for (i = 1; i < n - 1; i++) {
      pi = 0;
      b = 1;
      for (j = 0; j < 3; j++) {
        x = my_history()[i + j];
        pi += b * x;
        b *= 3;
      }
      if (i < n - 2) {
        mypat[pi][opp_history()[i + 3]] += 1;
        mypatCount[pi]++;
      }
    }
    for (i = 0; i < 27; i++)
      for (j = 0; j < 3; j++)
        mypat[i][j] = (mypat[i][j] + 1.0) / (mypatCount[i] + 3.0);

    for (j = 0; j < 3; j++) my_probs[j] += pat[pi][j];

    for (j = 0; j < 3; j++)
      if (mypat[pi][j] > 0.45) {
        if (j == kRock)
          return kPaper;
        else if (j == kPaper)
          return kScissors;
        else
          return kRock;
      }

    rstat += (probs[kRock] + my_probs[kRock]) / 3.0;
    pstat += (probs[kPaper] + my_probs[kPaper]) / 3.0;
    sstat += (probs[kScissors] + my_probs[kScissors]) / 3.0;

    return (biased_roshambo(sstat, rstat));
  }

 private:
  int rcount, pcount, scount;
};

}  // namespace roshambo_tournament

#endif  // INOCENCIO_H
