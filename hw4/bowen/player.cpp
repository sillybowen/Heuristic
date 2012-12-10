#include "player.h"
Player::Player(Board b,int l,int weight) {
  board = b;
  l = len;
  used_block_.resize(weight,false);
  opp_used_block_.resize(weight,false);
}
void Player::updateAdd(int pl,int p,int w) {
  board.add(pl,p,w);
}
void Player::updateRemove(int p) {
  board.remove(p);
}
