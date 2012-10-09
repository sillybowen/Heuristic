#ifndef FLAT_BOARD_HEADER_
#define FLAT_BOARD_HEADER_

#include "player.h"
#include "ClientSocket.h"
#include "SocketException.h"

#define NUMOFPLAYERS 2

class FlatBoard {
public:
  FlatBoard(int srv_port, int f1GamePos, int f2GamePos, int boardLen,
      int boardWeight);
  FlatBoard(int srv_port, int f1GamePos, int f2GamePos, int boardLen,
      int boardWeight, Player* otherPlayer);
  FlatBoard(int srv_port, int f1GamePos, int f2GamePos, int boardLen,
      int boardWeight, Player* firPlayer, Player* secPlayer);
  ~FlatBoard();
  int getFulcrumOnePos() const { return f1_pos_; }  // Return board index position
  int getFulcrumTwoPos() const { return f2_pos_; }  // Return board index position
  int getFulcrumOneValue() const { return f1_val_; }
  int getFulcrumTwoValue() const { return f2_val_; }
  Player* getP1() const { return ply_one_; }

  int addWt(int boardIndex, int weight);
  int removeWt(int boardIndex);
  int addGamePosWt(int gamePos, int weight);
  int startGame();
  void clientPlayer(std::string& teamName);
  int otherPlayerAdd(int gamePos, int weight);

  enum game_status { playing, oneWin, twoWin, equal };
  enum game_stage { addblock, removeblock };

private:
  ClientSocket* registerSrv(int srv_port);

  const int     board_len_;
  const int     f1_pos_;  // Relative to board arr index, from 0 to board_len_
  const int     f2_pos_;
  const int     board_wt_;
  int           f1_val_;
  int           f2_val_;
  int*          fl_board_;
  Player*       ply_one_;  // Owned here
  Player*       ply_two_;  // Owned here
  ClientSocket* arch_clt_;
};

#endif
