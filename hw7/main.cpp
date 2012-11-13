#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "game.h"
using namespace std;

//vector<Location*> locs;
int main(int argc, char* argv[]) {
  if (argc < 4) {
    puts("Format: ./main  teamName  numOfNanomunchers  srvOutputFile");
    return 1;
  }
  string s(argv[2]);
  int k;
  istringstream(argv[2])>>k;
  fprintf(stderr, "Global MAX numOfNanomunchers= %d\n", k);

  Game nanomuncherGame(argv[1], argv[3], k);
  nanomuncherGame.startGame();

  return 0;
}
