#include <cstdlib>
#include <iostream>
#include "server_game.h"
#include "person.h"
using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 5) {
    cout << "FORMAT: ./dating  teamName  N_Features  NumThrs  mode(0:local 1:server)\
srv_port(only for srv mode)" << endl;
    return 1;
  }
  int N = atoi(argv[2]), numThrs = atoi(argv[3]), mode = atoi(argv[4]), srv_port;
  if (mode == 1) {
    if (argc < 6) {
      cout << "Server Mode, must enter srv_port number" << endl;
      return 1;
    }
    srv_port = atoi(argv[5]);
  }

  Game* datingGame;
  if (mode) {  // Server mode, connect to judge
    cout << "Server Mode, plyName: " << argv[1] << "  N_Features= " << N
      << "  srv_port: " << srv_port << endl;
    datingGame = new ServerGame(argv[1], N, numThrs, srv_port);
  } else {  // Local test mode
    cout << "Local Mode, plyName: " << argv[1] << "  N_Features= " << N << endl;
    datingGame = new Game(argv[1], N, numThrs);
  }

  datingGame->startGame();
  // Person p("Person", N);
  // p.printExactW();

  delete datingGame;

  return 0;
}
