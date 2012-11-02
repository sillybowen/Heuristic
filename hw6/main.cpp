#include <cstdlib>
#include <climits>
#include <iostream>
#include "evasion.h"
#include "hunter.h"
#include "prey.h"
using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 6) {
    cout << "Format: ./evasion  srv_port  team_name  N_timesteps  M_maxWalls"
      << "  isHunter(0 or 1)" << endl;
    return 1;
  }

  int srv_port = atoi(argv[1]);
  string teamName(argv[2]);
  int Ntimesteps = atoi(argv[3]);
  int MmaxWalls = atoi(argv[4]);
  int isHunter = atoi(argv[5]);

  Moveable* pMyObj;
  if (isHunter) {
    pMyObj = new Hunter(0, 0, Ntimesteps, MmaxWalls);
    cout << "Hunter clt N= " << Ntimesteps << " ;M= " << MmaxWalls
      << " ;teamName= " << teamName << " ;srv_port= " << srv_port << endl;
  } else {
    pMyObj = new Prey(330, 200, Ntimesteps, MmaxWalls);
    cout << "Prey clt N= " << Ntimesteps << " ;M= " << MmaxWalls
      << " ;teamName= " << teamName << " ;srv_port= " << srv_port << endl;
  }
  Evasion* pEva = new Evasion(pMyObj, Ntimesteps, MmaxWalls, srv_port);
  pEva->dumpAdjWallsMatrixGraph();

  // Start game
  pEva->startGame(teamName);

  // Free memory
  delete pEva;
  return 0;
}
