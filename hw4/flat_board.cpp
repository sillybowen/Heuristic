#include <cstring>
#include <iostream>
#include "flat_board.h"
#include "greedy_player.h"

using std::cout;
using std::endl;

FlatBoard::FlatBoard(int f1GamePos, int f2GamePos, int boardLen, int boardWeight)
  : board_len_(boardLen),
    f1_pos_(f1GamePos + board_len_ / 2),
    f2_pos_(f2GamePos + board_len_ / 2),
    board_wt_(boardWeight),
    f1_val_((f1_pos_ - board_len_ / 2) * board_wt_),
    f2_val_((f2_pos_ - board_len_ / 2) * board_wt_),
    fl_board_(new int[board_len_ + 1]),
    ply_one_(new GreedyPlayer(this, fl_board_, board_len_)),
    ply_two_(new GreedyPlayer(this, fl_board_, board_len_)) {
  memset(fl_board_, 0, sizeof(fl_board_));
}

// Transfer ownship to FlatBoard class
FlatBoard::FlatBoard(int f1GamePos, int f2GamePos, int boardLen, int boardWeight,
    Player* otherPlayer) : board_len_(boardLen),
                           f1_pos_(f1GamePos + board_len_ / 2),
                           f2_pos_(f2GamePos + board_len_ / 2),
                           board_wt_(boardWeight),
                           f1_val_((f1_pos_ - board_len_ / 2) * board_wt_),
                           f2_val_((f2_pos_ - board_len_ / 2) * board_wt_),
                           fl_board_(new int[board_len_ + 1]),
                           ply_one_(new GreedyPlayer(this, fl_board_, board_len_)),
                           ply_two_(otherPlayer) {
  memset(fl_board_, 0, sizeof(fl_board_));
  ply_two_->linktoFlatBoard(this, fl_board_, board_len_);
}

FlatBoard::~FlatBoard() {
  delete ply_one_;
  delete ply_two_;
  delete [] fl_board_;
}

// Return -1 if illegal move, otherwise 1
int FlatBoard::addWt(int boardIndex, int weight) {
  if (fl_board_[boardIndex]) {  // Already occupied
    return -1;
  } else {
    fl_board_[boardIndex] = weight;
    f1_val_ += (f1_pos_ - boardIndex) * weight;
    f2_val_ += (f2_pos_ - boardIndex) * weight;
    return 1;
  }
}

// Have to transfer gamePos(-15, to +15) to proper index on fl_board_ (gamePos + 15)
int FlatBoard::addGamePosWt(int gamePos, int weight) {
  int boardIndex = gamePos + board_len_ / 2;
  return addWt(boardIndex, weight);
}

int FlatBoard::startGame() {
  bool turn = true;  // It's true if it is player one's turn
  int gameResult = playing;

  while (!gameResult) {
    if (turn) {  // ply_one_ add weight
      Player::MovePos onemove = ply_one_->nextAdd();

      if (onemove.wt == 0)  // All blocks are used
        return equal;

      cout << "Player One move: pos-> " << onemove.boardArrPos - board_len_ / 2
        << " weight-> " << onemove.wt << endl;
      // Move on to the board
      addWt(onemove.boardArrPos, onemove.wt);
      cout << "f1val= " << f1_val_ << " f2val= " << f2_val_ << endl;
      if (f1_val_ > 0 || f2_val_ < 0) {
        cout << "Player One FAILED" << endl;
        gameResult = twoWin;
      }
    } else {
      Player::MovePos twomove = ply_two_->nextAdd();
      cout << "Player Two move: pos-> " << twomove.boardArrPos - board_len_ / 2
        << " weight-> " << twomove.wt << endl;
      // Move on to the board
      addWt(twomove.boardArrPos, twomove.wt);
      cout << "f1val= " << f1_val_ << " f2val= " << f2_val_ << endl;
      if (f1_val_ > 0 || f2_val_ < 0) {
        cout << "Player Two FAILED" << endl;
        gameResult = oneWin;
      }
    }
    turn = !turn;
  }

  return gameResult;
}

// Return -1 if illegal move, otherwise 1
// Have to transfer gamePos(-15, to +15) to proper index on fl_board_ (gamePos + 15)
int FlatBoard::otherPlayerAdd(int gamePos, int weight) {
  int boardIndex = gamePos + board_len_ / 2;
  if (fl_board_[boardIndex]) {  // Already occupied
    return -1;
  } else {
    fl_board_[boardIndex] = weight;
    return 1;
  }
}
