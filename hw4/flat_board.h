#ifndef FLAT_BOARD_HEADER_
#define FLAT_BOARD_HEADER_

#include "player.h"

#define NUMOFPLAYERS 2

class FlatBoard {
public:
  FlatBoard(int f1GamePos, int f2GamePos, int boardLen, int boardWeight);
  FlatBoard(int f1GamePos, int f2GamePos, int boardLen, int boardWeight,
      Player* otherPlayer);
  ~FlatBoard();
  int getFulcrumOnePos() const { return f1_pos_; }  // Return board index position
  int getFulcrumTwoPos() const { return f2_pos_; }  // Return board index position
  int getFulcrumOneValue() const { return f1_val_; }
  int getFulcrumTwoValue() const { return f2_val_; }
  Player* getP1() const { return ply_one_; }

  int addWt(int boardIndex, int weight);
  int addGamePosWt(int gamePos, int weight);
  int startGame();
  int otherPlayerAdd(int gamePos, int weight);

  enum game_status { playing, oneWin, twoWin, equal };

private:
  const int board_len_;
  const int f1_pos_;  // Relative to board arr index, from 0 to board_len_
  const int f2_pos_;
  const int board_wt_;
  int       f1_val_;
  int       f2_val_;
  int*      fl_board_;
  Player*   ply_one_;  // Owned here
  Player*   ply_two_;  // Owned here
};

#endif
