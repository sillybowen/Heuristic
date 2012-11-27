#include <cstdlib>
#include <iostream>
#include "game.h"
using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 4) {
    cout << "FORMAT: ./portfolio  teamName  port#  mode(1:stable 2:unstable)  user(1:Bowen 2:Tao 3:Jinil) " << endl;
    return 1;
  }
  int srv_port = atoi(argv[2]), mode = atoi(argv[3]), user = atoi(argv[4]);
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

  portfolioGame->startGame(user);

  delete portfolioGame;

  return 0;
}
