#include <cstdlib>
#include <iostream>
#include "game.h"
using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cout << "FORMAT: ./portfolio  teamName  port#  mode(1:stable class 2:unstable class) " << endl;
    return 1;
  }
  int srv_port = atoi(argv[2]), mode = atoi(argv[3]);
  Game* portfolioGame = new Game(argv[1], srv_port, mode);

  // the set of favorable classes and unfavorable classes will stay the same over all rounds
  if (mode == 1) {  
    cout << "Mode-1, plyName: " << argv[1] << "  port: " << srv_port << endl;
  } 
  // the set of favorable and unfavorable classes will change up to 10 times
  else if(mode == 2) {  // Local test mode
    cout << "Mode-1, plyName: " << argv[1] << "  port: " << srv_port << endl;
  }
  else{
    cout << "Wrong input mode number. Please select 1 or 2." << endl;
    return 1;
  }

  portfolioGame->startGame();

  delete portfolioGame;

  return 0;
}
