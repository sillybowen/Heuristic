#ifndef minmax_player_h_
#define minmax_player_h_
#include "player.h"

class MinMaxPlayer : public Player {
 public:
  MinMaxPlayer (Board b,int len,int weight);

  void nextAdd(int &pos,int &w);
  void nextRemove(int &p);
  int score(){return 0};
 private:
  int minAddStep(int d);
  int maxAddStep(int d,int &pos,int &w);
  int minRemoveStep(int d);
  int maxRemoveStep(int d);
  vector<int> blocks_arr_;
  vector<int> opp_blocks_arr_;
  FlatBoard localboard;
};
#endif
