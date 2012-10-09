#include <cstdlib>
#include <climits>
#include <iostream>
#include "flat_board.h"
#include "greedy_player.h"
#include "human_player.h"
#include "dp_remove.h"
using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 4) {
    cout << "Format: ./notipping  srv_port  team_name  is_human(default no:0)"
      << endl;
    return 1;
  }

  int srv_port = atoi(argv[1]), ishuman = 0;
  string teamName(argv[2]);
  Player* human = NULL;
  FlatBoard* pfb;
  if (argc == 4)
    ishuman = atoi(argv[3]);
  // Board length must be Even number, so we can get equal half length
  // FlatBoard fb(srv_port, -3, -1, 30, 3);
  if (ishuman) {
    human = new HumanPlayer();
    pfb = new FlatBoard(srv_port, -3, -1, 30, 3, human);
  } else {
    pfb = new FlatBoard(srv_port, -3, -1, 30, 3);
  }

  cout << "Only Board Fulcrum values: " << pfb->getFulcrumOneValue() << ", "
    << pfb->getFulcrumTwoValue() << endl;
  pfb->addGamePosWt(-4, 3);
  cout << "Init Fulcrum values: " << pfb->getFulcrumOneValue() << ", "
    << pfb->getFulcrumTwoValue() << endl;

  // pfb->startGame();
  pfb->clientPlayer(teamName);

  // example how to used dp_remove
  /*
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
  */
  return 0;
}
