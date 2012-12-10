#include <iostream>
#include <limits>
#include "minmax_player.h"
using namespace std;

MinMaxPlayer:: MinMaxPlayer( Board b,int len,int weight) : Player(b,len,weight) {
}

Player::MovePos MinMaxPlayer::nextAdd(int &pos,int &w) {
  maxAddStep(2,pos,w);
  return ret;
}

int MinMaxPlayer::nextRemove() {
  int ret = maxRemoveStep(2);
  return ret;
}

int MinMaxPlayer::minAddStep(int d) {

}
int MinMaxPlayer::maxAddStep(int d, Player::MovePos& mv) {
  int sc = std::numeric_limits<int>::min();
  int answ,ansp;
  int tmp;
  for (int w = 1; w<= PLAYERNUMOFBLOCKS;w++)
    if (blocks_arr_[w] == 1)
      for (int p = 0;p<boardLen_;p++)
        if (pflat_board_->validateAdd(p,w)){
          pflat_board_->addWt(p,w);
          if (d>0)
            tmp = minAddStep(d-1);
          else
            tmp = score();
          if (tmp>sc) {
            answ = w;
            ansp = p;
            sc = tmp;
          }
          pflat_board_->removeWt(p);
        }
  mv.boardArrPos = ansp;
  mv.wt = answ;
}
int MinMaxPlayer::minRemoveStep(int d) {

}
int MinMaxPlayer::minRemoveStep(int d) {

}
