#ifndef init_choice_h
#define init_choice_h
#include <vector>
#include "gamble.h"
using std::vector;

class InitChoice {
public:
  InitChoice(int numGambles, int numLinks, const vector<Gamble*>& gambles,
      const vector<Link*>& links);
  ~InitChoice();
  void printClassBelongMap() const;

private:
  void calExpVarsOfGambles();
  void initLinksMatrGraph();

  const int              n_gambles_;
  const int              n_links_;
  int**                  link_matr_;
  int*                   gTo_cla_;  // Given gamble_id, find class_id
  vector<int>*           claTo_g_;  // Given class_id, find all gamble_ids in it
  const vector<Gamble*>* g_data_;  // Passed in gamble data
  const vector<Link*>*   l_data_;  // Passed in link data
  vector<double>         exp_g_;   // Expected return of each gamble
  vector<double>         var_g_;   // Variance of each gamble
};

#endif
