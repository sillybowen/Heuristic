#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "flat_board.h"
#include "greedy_player.h"
#include "dp_remove.h"

using std::cout;
using std::endl;
using std::setw;
using std::stringstream;

FlatBoard::FlatBoard(int srv_port, int f1GamePos, int f2GamePos, int boardLen,
    int boardWeight) : board_len_(boardLen),
                       f1_pos_(f1GamePos + board_len_ / 2),
                       f2_pos_(f2GamePos + board_len_ / 2),
                       board_wt_(boardWeight),
                       f1_val_((f1_pos_ - board_len_ / 2) * board_wt_),
                       f2_val_((f2_pos_ - board_len_ / 2) * board_wt_),
                       fl_board_(new int[board_len_ + 1]),
                       ply_one_(new GreedyPlayer(this, fl_board_, board_len_)),
                       ply_two_(new GreedyPlayer(this, fl_board_, board_len_)),
                       arch_clt_(registerSrv(srv_port)) {
  memset(fl_board_, 0, sizeof(fl_board_));
}

// Transfer ownship to FlatBoard class, then link second player to FlatBoard class
FlatBoard::FlatBoard(int srv_port, int f1GamePos, int f2GamePos, int boardLen,
    int boardWeight, Player* otherPlayer)
    : board_len_(boardLen),
    f1_pos_(f1GamePos + board_len_ / 2),
    f2_pos_(f2GamePos + board_len_ / 2),
    board_wt_(boardWeight),
    f1_val_((f1_pos_ - board_len_ / 2) * board_wt_),
    f2_val_((f2_pos_ - board_len_ / 2) * board_wt_),
    fl_board_(new int[board_len_ + 1]),
    ply_one_(new GreedyPlayer(this, fl_board_, board_len_)),
    ply_two_(otherPlayer),
    arch_clt_(registerSrv(srv_port)) {
  memset(fl_board_, 0, sizeof(fl_board_));
  ply_two_->linktoFlatBoard(this, fl_board_, board_len_);
}

// Transfer two players' ownship to FlatBoard class,
// then link two players to FlatBoard class
FlatBoard::FlatBoard(int srv_port, int f1GamePos, int f2GamePos, int boardLen,
    int boardWeight, Player* firPlayer, Player* secPlayer)
    : board_len_(boardLen),
      f1_pos_(f1GamePos + board_len_ / 2),
      f2_pos_(f2GamePos + board_len_ / 2),
      board_wt_(boardWeight),
      f1_val_((f1_pos_ - board_len_ / 2) * board_wt_),
      f2_val_((f2_pos_ - board_len_ / 2) * board_wt_),
      fl_board_(new int[board_len_ + 1]),
      ply_one_(firPlayer),
      ply_two_(secPlayer),
      arch_clt_(registerSrv(srv_port)) {
  memset(fl_board_, 0, sizeof(fl_board_));
  ply_one_->linktoFlatBoard(this, fl_board_, board_len_);
  ply_two_->linktoFlatBoard(this, fl_board_, board_len_);
}

FlatBoard::~FlatBoard() {
  delete ply_one_;
  delete ply_two_;
  delete arch_clt_;
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

// Return -1 if illegal remove, otherwise 1
// Also update f1, f2 values
int FlatBoard::removeWt(int boardIndex) {
  if (fl_board_[boardIndex] <= 0) {  // Illegal remove
    return -1;
  } else {
    int weight = fl_board_[boardIndex];
    fl_board_[boardIndex] = 0;
    f1_val_ -= (f1_pos_ - boardIndex) * weight;
    f2_val_ -= (f2_pos_ - boardIndex) * weight;
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
  int plystage = addblock;
  int gameResult = playing;

  while (!gameResult) {
    Player::MovePos addmove(0);
    int removepos;
    if (turn) {  // ply_one_ add weight
      if (plystage == addblock)
        addmove = ply_one_->nextAdd();
      else
        removepos = ply_one_->nextRemove();

      if (plystage == addblock && addmove.wt == 0) {  // All blocks are used
        plystage = removeblock;
        continue;
      }
      if (plystage == removeblock && removepos == -1) {  // All blocks are removed
        gameResult = equal;
        continue;
      }

      if (plystage == addblock) {
        // Move on to the board
        assert(addWt(addmove.boardArrPos, addmove.wt) != -1);
        cout << "Player One add: pos-> " << addmove.boardArrPos - board_len_ / 2
          << " weight-> " << addmove.wt << endl;
      } else {
        assert(removeWt(removepos) != -1);
        cout << "Player One Remove: pos-> " << removepos - board_len_ / 2 << endl;
      }
    } else {
      if (plystage == addblock) {
        addmove = ply_two_->nextAdd();
        // Move on to the board
        assert(addWt(addmove.boardArrPos, addmove.wt) != -1);
        cout << "Player Two add: pos-> " << addmove.boardArrPos - board_len_ / 2
          << " weight-> " << addmove.wt << endl;
      } else {
        removepos = ply_two_->nextRemove();
        assert(removeWt(removepos) != -1);
        cout << "Player Two Remove: pos-> " << removepos - board_len_ / 2 << endl;
      }
    }

    cout << "f1val= " << f1_val_ << " f2val= " << f2_val_ << endl;
    if (f1_val_ > 0 || f2_val_ < 0) {
      if (turn) {
        cout << "Player One FAILED" << endl;
        gameResult = oneWin;
      } else {
        cout << "Player Two FAILED" << endl;
        gameResult = twoWin;
      }
    }
    turn = !turn;
  }

  return gameResult;
}

// Only use ply_two_;
void FlatBoard::clientPlayer(std::string& teamName) {
  bool firstRemove = true;
  std::string fromSrv, fromPly;
  DPRemove* pdpRemove = NULL;

  teamName.push_back('\n');
  Player::MovePos amove(0);

  try {
    (*arch_clt_) << teamName;

    do {
      (*arch_clt_) >> fromSrv;
      if (fromSrv.empty() || fromSrv.compare("Bye") == 0)
        break;
      cout << "#Server: \"" << fromSrv << "\"\n";

      if (fromSrv.compare(0, 3, "ADD") == 0) {
        readSrvUpdateBoard(fromSrv);
        outputboard();
        amove = ply_two_->nextAdd();
        stringstream ss;
        ss << amove.wt << ',' << amove.boardArrPos - board_len_ / 2 << '\n';
        cout << "Add to pos: " << amove.boardArrPos - board_len_ / 2
          << " Wt = " << amove.wt
          << " Current val at this pos: " << fl_board_[amove.boardArrPos] << endl;
        fromPly = ss.str();

        if (!fromPly.empty()) {
          (*arch_clt_) << fromPly;
        }
      } else if (fromSrv.compare(0, 6, "REMOVE") == 0) {
        if (firstRemove) {
          pdpRemove = new DPRemove(board_len_ + 1, fl_board_);
          pdpRemove->work();
          firstRemove = false;
        }
        readSrvUpdateBoard(fromSrv);
        // amove.boardArrPos = ply_two_->nextRemove();
        amove.boardArrPos = pdpRemove->decision(fl_board_);
        cout << "Remove to pos: " << amove.boardArrPos - board_len_ / 2
          << " Wt = " << fl_board_[amove.boardArrPos]
          << " Current val at this pos: " << fl_board_[amove.boardArrPos] << endl;
        stringstream ss;
        ss << fl_board_[amove.boardArrPos] << ','
          << amove.boardArrPos - board_len_ / 2 << '\n';
        fromPly = ss.str();

        if (!fromPly.empty()) {
          (*arch_clt_) << fromPly;
        }
      } else {
        continue;
      }

    } while (1);

  } catch ( SocketException& ) {
    assert(0);
  }

  if (pdpRemove != NULL)
    delete pdpRemove;
}

void FlatBoard::outputboard() const {
  for (int i = 0; i <= board_len_; ++i)
    cout << setw(3) << i - board_len_ / 2 << ' ';
  cout << endl;
  
  for (int i = 0; i <= board_len_; ++i)
    cout << setw(3) << fl_board_[i] << ' ';
  cout << endl;
}

void FlatBoard::readSrvUpdateBoard(const std::string& fromSrv) {
  stringstream ss;
  ss << fromSrv;

  int tmp, wt, gamePos, ind = 0;
  while ((tmp = ss.get()) != '|');

  while (ss.peek() != '|') {
    ss >> wt;
    assert(ss.get() == int(','));
    ss >> gamePos;

    while (ind < (gamePos + board_len_ / 2))
      fl_board_[ind++] = 0;

    fl_board_[gamePos + board_len_ / 2] = wt;

    ++ind;
  }
  while (ind <= board_len_)
    fl_board_[ind++] = 0;

  // Sum up to get f1crum and f2crum values
  f1_val_ = 0;
  f2_val_ = 0;
  for (int i = 0; i <= board_len_; ++i) {
    f1_val_ += (f1_pos_ - i) * fl_board_[i];
    f2_val_ += (f2_pos_ - i) * fl_board_[i];
  }
}


// Register with the architecture server
ClientSocket* FlatBoard::registerSrv(int srv_port) {
  ClientSocket* pClt_socket = NULL;

  try {
    pClt_socket = new ClientSocket("localhost", srv_port);
  } catch (SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << " PortNum: "
      << srv_port << std::endl;
    assert(0);
  }

  return pClt_socket;
}
