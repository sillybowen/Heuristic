#include "hunter.h"

using std::cout;
using std::endl;

void Hunter::output() const {
  cout << "Hunter pos: (" << x << ", " << y << ")" << endl;
}

bool Hunter::isHunter() const {
  return true;
}

bool Hunter::tryMove() {
}
