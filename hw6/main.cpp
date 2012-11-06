#include <cstdlib>
#include <climits>
#include <iostream>
#include "evasion.h"
#include "hunter.h"
#include "prey.h"
#include "bowen_hunter.h"
#include "bowen_prey.h"
#include "jinil_hunter.h"
#include "jinil_prey.h"
#include "random_prey.h"
using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 6) {
    cout << "Format: ./evasion  srv_port  team_name  N_timesteps  M_maxWalls  \
testType[0:Tao, 1:Bowen-randPrey, 2:Jinil 3:randomPreyTest]"
      << "  isHunter(0 or 1)" << endl;
    return 1;
  }

  int srv_port = atoi(argv[1]);
  string teamName(argv[2]);
  int Ntimesteps = atoi(argv[3]);
  int MmaxWalls = atoi(argv[4]);
  int testType = atoi(argv[5]);
  int isHunter = atoi(argv[6]);


  Moveable* pMyObj;
  switch (testType) {
  case 0:              // Tao version
    if (isHunter) {
      pMyObj = new Hunter(0, 0, Ntimesteps, MmaxWalls);
      cout << "Hunter clt N= " << Ntimesteps << " ;M= " << MmaxWalls
	   << " ;teamName= " << teamName << " ;srv_port= " << srv_port << endl;
    } else {
      pMyObj = new Prey(330, 200, Ntimesteps, MmaxWalls);
      cout << "Prey clt N= " << Ntimesteps << " ;M= " << MmaxWalls
	   << " ;teamName= " << teamName << " ;srv_port= " << srv_port << endl;
    }
    break;

  case 1:              // Bowen version - with random prey
    if (isHunter) {
      pMyObj = new Bowen_Hunter(0, 0, Ntimesteps, MmaxWalls);
      cout << "Hunter clt N= " << Ntimesteps << " ;M= " << MmaxWalls
	   << " ;teamName= " << teamName << " ;srv_port= " << srv_port << endl;
    } else {
      pMyObj = new Random_Prey(330, 200, Ntimesteps, MmaxWalls);
      cout << "Prey clt N= " << Ntimesteps << " ;M= " << MmaxWalls
	   << " ;teamName= " << teamName << " ;srv_port= " << srv_port << endl;
    }
    break;

  case 2:              // Jinil version
    if (isHunter) {
      pMyObj = new Jinil_Hunter(0, 0, Ntimesteps, MmaxWalls);
      cout << "Hunter clt N= " << Ntimesteps << " ;M= " << MmaxWalls
	   << " ;teamName= " << teamName << " ;srv_port= " << srv_port << endl;
    } else {
      pMyObj = new Jinil_Prey(330, 200, Ntimesteps, MmaxWalls);
      cout << "Prey clt N= " << Ntimesteps << " ;M= " << MmaxWalls
	   << " ;teamName= " << teamName << " ;srv_port= " << srv_port << endl;
    }
    break;

  case 3:             // Random Prey version with Jinil hunter
    if (isHunter) {
      pMyObj = new Jinil_Hunter(0, 0, Ntimesteps, MmaxWalls);
      cout << "Hunter clt N= " << Ntimesteps << " ;M= " << MmaxWalls
	   << " ;teamName= " << teamName << " ;srv_port= " << srv_port << endl;
    } else {
      pMyObj = new Random_Prey(330, 200, Ntimesteps, MmaxWalls);
      cout << "Prey clt N= " << Ntimesteps << " ;M= " << MmaxWalls
	   << " ;teamName= " << teamName << " ;srv_port= " << srv_port << endl;
    }
  }

  Evasion* pEva = new Evasion(pMyObj, Ntimesteps, MmaxWalls, srv_port);
  pEva->dumpAdjWallsMatrixGraph();

  // Start game
  pEva->startGame(teamName);

  // Free memory
  delete pEva;
  return 0;
}
