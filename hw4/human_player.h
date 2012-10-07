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
    cout << "Enter: gamePos  weight" << endl;
    cin >> gamePos >> wt;
    MovePos aMove(gamePos + boardLen_ / 2, wt);
    return aMove;
  }
};

#endif
