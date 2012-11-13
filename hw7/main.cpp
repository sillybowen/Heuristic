#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "game.h"
using namespace std;

//vector<Location*> locs;
int main(int argc, char* argv[]) {
  if (argc < 4) {
    puts("Format: ./main  teamName  numOfNanomunchers  srvOutputFile  srvPort");
    return 1;
  }
  string s(argv[2]);
  int k, srv_port;
  istringstream(argv[2])>>k;
  srv_port = atoi(argv[4]);
  fprintf(stderr, "Global MAX numOfNanomunchers= %d  SrvPort: %d\n", k, srv_port);

  Game nanomuncherGame(argv[1], argv[3], k, srv_port);
  nanomuncherGame.startGame();

  return 0;
}
