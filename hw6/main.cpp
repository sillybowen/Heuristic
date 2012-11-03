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

  /* Test for wall-adjacency function
  char tmp = ' ';
  int x1, y1, x2, y2;
  Moveable::Wall staticWall(100, 100, 200, 100, 0);
  while (tmp != 'q') {
    cout << "Enter a wall: x1 y1  x2 y2: ";
    cin >> x1 >> y1 >> x2 >> y2;
    Moveable::Wall newWall(x1, y1, x2, y2, 1);
    cout << "Created a wall: (" << newWall.x1 << ", " << newWall.y1 << ") -> ("
      << newWall.x2 << ", " << newWall.y2 << ")  Contact? "
      << staticWall.isTwoWallAdjacent(&newWall, x1, y1, x2, y2) << endl;
    cout << "Enter anything to continue, q to quit: ";
    cin >> tmp;
  }
  */

  // Start game
  pEva->startGame(teamName);

  // Free memory
  delete pEva;
  return 0;
}
