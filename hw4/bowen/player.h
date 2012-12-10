#ifndef player_h_
#define player_h_
#include "board.h"
using namespace std;
class Player {
 private:
  Board board;
  vector<bool> used_block_;
  vector<bool> opp_used_block_;
  int f1,f2;
  int player_id;// 1 or 2
  int len;
 public:
  Player(Board b,int len,int weight);
  virtual void nextAdd(int &pos,int &w) = 0;
  virtual void nextRemove(int &pos) = 0;
  void updateAdd(int pl,int p, int w);
  void updateRemove(int p);
};
#endif
