#ifndef GREEDY_PLAYER_HEADER_
#define GREEDY_PLAYER_HEADER_

#include "player.h"

class GreedyPlayer : public Player {
public:
  GreedyPlayer(const FlatBoard* pFB, const int* pboard, int boardLen);
  ~GreedyPlayer();

  MovePos nextAdd();
  void fullScoreDistri() const;
  void getScoreDistri(int weight) const;

private:
  MovePos greedyAdd();
  int scoreFunct(MovePos aMove) const;
  int getLargestAvailBlock() const;

  const int  f1_pos_;
  const int  f2_pos_;
  int blocks_arr_[PLAYERNUMOFBLOCKS + 1];  //1 at index 3 mean 3kg block is available
};

#endif
