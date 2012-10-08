#include <climits>
#include <iostream>
#include "flat_board.h"
#include "greedy_player.h"
#include "human_player.h"

using namespace std;

int main(int argc, char* argv[]) {
  // Board length must be Even number, so we can get equal half length
  cout << "INT_MIN= " << INT_MIN << endl;

  FlatBoard fb(-3, -1, 20, 3);
  // Player* human = new HumanPlayer();
  // FlatBoard fb(-3, -1, 20, 3, human);
  cout << "Only Board Fulcrum values: " << fb.getFulcrumOneValue() << ", "
    << fb.getFulcrumTwoValue() << endl;
  fb.addGamePosWt(-4, 3);
  cout << "Init Fulcrum values: " << fb.getFulcrumOneValue() << ", "
    << fb.getFulcrumTwoValue() << endl;

  // GreedyPlayer* pGreedy = dynamic_cast<GreedyPlayer*>(fb.getP1());
  // // pGreedy->fullScoreDistri();
  // cout << "Add to pos: " << pGreedy->greedyAdd().boardArrPos << " Weight: "
  //   << pGreedy->greedyAdd().wt << endl;

  fb.startGame();
  return 0;
}
