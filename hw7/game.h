#ifndef game_h
#define game_h
#include <string>

using std::string;

class Game {
public:
  Game(const char* plyName, const char* srvOutFile, int globalMaxK)
    : max_k_(globalMaxK), ply_name_(string(plyName)), inFile_srv_(srvOutFile) { }
  ~Game() { }

  void startGame();
  bool readSrvOutput();

private:
  const int    max_k_;  // Max num of nanomunchers in global scale
  const string ply_name_;  // Player I am in control
  const string inFile_srv_;  // input file from server (server send messages here)
};

#endif  // game_h
