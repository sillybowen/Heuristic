#include <climits>
#include <iostream>
#include "flat_board.h"
#include "greedy_player.h"
#include "human_player.h"
#include "dp_remove.h"
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

  // example how to used dp_remove
  int boardLen = 31;
  int x[31];
  for (int i = 0; i<31;i++)
    x[i] = 0;
  x[11]=3;
  x[12]=12;
  x[14] =12;
  for (int i = 0; i<11;i++) {
    x[i] = i+1;
    x[26-i] = i+1;
  }
  DPRemove dd(boardLen,x);
  dd.work();
  int move = 0;
  while (move !=-1) {
    move = dd.decision(x);
    cout<<move<<endl;
    if (move!=-1) {
      x[move] = 0;
    }
  }
  return 0;
}
