#ifndef PLAYER_HEADER_
#define PLAYER_HEADER_

#define PLAYERNUMOFBLOCKS 12

class FlatBoard;

class Player {
public:
  Player() : pflat_board_(NULL), pfl_board_(NULL), boardLen_(-1) { }
  Player(const FlatBoard* pFB, const int* pboard, int boardLen)
    : pflat_board_(pFB), pfl_board_(pboard),
      boardLen_(boardLen) { }
  virtual ~Player() { }

  struct MovePos {
    int boardArrPos;  // pos relative to arr index, from 0 to boardLen_
    int wt;
    MovePos(int weight) : boardArrPos(0), wt(weight) { }
    MovePos(int boardPos, int weight) : boardArrPos(boardPos), wt(weight) { }
  };

  virtual MovePos nextAdd() = 0; // Add block, using board index pos
  virtual int nextRemove() = 0;  // Remove an existing block, return board index pos

  void linktoFlatBoard(const FlatBoard* pFB, const int* pboard, int boardLen) {
    pflat_board_ = pFB;
    pfl_board_ = pboard;
    boardLen_ = boardLen;
  }

protected:
  const FlatBoard* pflat_board_;
  const int* pfl_board_;  // Array recording the status of the flat board
  int  boardLen_;
};

#endif
