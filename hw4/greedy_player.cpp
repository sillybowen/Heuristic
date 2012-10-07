#include <cassert>
#include <cstring>
#include <climits>
#include <iostream>
#include <iomanip>
#include "greedy_player.h"
#include "flat_board.h"

using std::cout;
using std::endl;
using std::setw;

GreedyPlayer::GreedyPlayer(const FlatBoard* pFB, const int* pboard, int boardLen)
  : Player(pFB, pboard, boardLen),
    f1_pos_(pflat_board_->getFulcrumOnePos()),
    f2_pos_(pflat_board_->getFulcrumTwoPos()) {
  blocks_arr_[0] = 0;
  for (int i = 1; i <= PLAYERNUMOFBLOCKS; ++i)
    blocks_arr_[i] = 1;
}

GreedyPlayer::~GreedyPlayer() { }

Player::MovePos GreedyPlayer::nextAdd() {
  return greedyAdd();
}

Player::MovePos GreedyPlayer::greedyAdd() {
  int wtblock = getLargestAvailBlock();
  if (wtblock == 0) {  // All blocks are used
    return MovePos(0);
  }

  blocks_arr_[wtblock] = 0;  // Get the largest block
  MovePos nextMove = MovePos(wtblock);

  int largestScore = 0;
  int chosePos = -1;
  for (int i = 0; i < boardLen_; ++i) {
    int tmp;
    nextMove.boardArrPos = i;
    if (!pfl_board_[i] && (tmp = scoreFunct(nextMove)) > largestScore) {
      largestScore = tmp;
      chosePos = i;
    }
  }

  assert(largestScore != 0);

  nextMove.boardArrPos = chosePos;
  return nextMove;
}

int GreedyPlayer::scoreFunct(MovePos aMove) const {
  int curf1val = pflat_board_->getFulcrumOneValue();
  int curf2val = pflat_board_->getFulcrumTwoValue();

  curf1val += (f1_pos_ - aMove.boardArrPos) * aMove.wt;
  curf2val += (f2_pos_ - aMove.boardArrPos) * aMove.wt;

  if (curf1val > 0 || curf2val < 0)
    return INT_MIN;
  else
    return -curf1val + curf2val - curf1val * curf2val;  // |x| + |y| + |x*y|
}

int GreedyPlayer::getLargestAvailBlock() const {
  int i;
  for (i = PLAYERNUMOFBLOCKS; i > 0; --i) {
    if (blocks_arr_[i])
      return i;
  }
  return i;  // Return 0 if all blocks are used
}

void GreedyPlayer::fullScoreDistri() const {
  cout << "Pos:       ";
  for (int i = 0; i <= boardLen_; ++i)
    cout << " " << setw(4) << i - boardLen_ / 2 << " ";
  cout << endl;

  for (int i = 1; i <= PLAYERNUMOFBLOCKS; ++i) {
    cout << "Weight " << i << " : ";
    getScoreDistri(i);
  }
}

void GreedyPlayer::getScoreDistri(int weight) const {
  MovePos tmpMove(weight);
  for (int i = 0; i <= boardLen_; ++i) {
    tmpMove.boardArrPos = i;
    int tmp;
    if ((tmp = scoreFunct(tmpMove)) == INT_MIN) {
      cout << " FAIL ";
    } else {
      cout << " " << setw(4) << tmp << " ";
    }
  }
  cout << endl;
}
