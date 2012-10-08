#ifndef HUMAN_PLAYER_HEADER_
#define HUMAN_PLAYER_HEADER_

#include <iostream>
#include "player.h"

using std::cout;
using std::endl;
using std::cin;

class HumanPlayer : public Player {
public:
  HumanPlayer() { }
  ~HumanPlayer() { }

  MovePos nextAdd() {
    int gamePos, wt;
    do {
      cout << "Human player enter: gamePos  weight ";
      cin >> gamePos >> wt;
    } while (pfl_board_[gamePos + boardLen_ / 2] &&
        (cout << "\nAlready occupied, reenter: "));

    MovePos aMove(gamePos + boardLen_ / 2, wt);
    return aMove;
  }

  int nextRemove() {
    int gamePos;

    do {
      cout << "Human player enter: remove_gamePos: ";
      cin >> gamePos;
    } while (!pfl_board_[gamePos + boardLen_ / 2] &&
        (cout << "\nNo block to remove, reenter: "));

    return (gamePos + boardLen_ / 2);
  }
};

#endif
