#include <cstring>
#include <iostream>
#include <iomanip>
// #include "hunter.h"
#include "prey.h"
#include "evasion.h"
using std::cout;
using std::endl;
using std::setw;

Prey::Prey(int px, int py, int nn, int mm) : Moveable(px, py, nn, mm),
    MaxNumOfSteps_(InitHunterPredPathDanger) {
  for (int i = 0; i <= 500; ++i) {
    memset(dangerMap[i], 0, sizeof(int) * 501);
  }
}

Prey::~Prey() { }

void Prey::output() const {
  for (int i = 0; i <= 500; ++i) {
    for (int j = 0; j <= 500; ++j) {
      cout << setw(6) << dangerMap[i][j];
    }
    cout << endl;
  }
}

bool Prey::isHunter() const { return false; }

Moveable::HuntPreyOutput Prey::tryMove() {
  Pos hunterActualCur = evade_game_->h_pos;
  vector<Pos> hFutureSteps;
  int h_dir_x, h_dir_y;

  int stepsWent = Moveable::hunterNStepPrediction(MaxNumOfSteps_, hFutureSteps,
      evade_game_->hor_walls_, evade_game_->ver_walls_, hunterActualCur,
      evade_game_->h_pos_history_, h_dir_x, h_dir_y);

  preyAddDangerOnHunterPredPath(stepsWent, hFutureSteps);

  // Get hunter's actual current direction
  Moveable::hunterNStepPrediction(2, hFutureSteps,
      evade_game_->hor_walls_, evade_game_->ver_walls_, hunterActualCur,
      evade_game_->h_pos_history_, h_dir_x, h_dir_y);
  // Add preference(counter danger) along hunter's direction
  preyAddPreferOnHunterDir(h_dir_x, h_dir_y, hunterActualCur, evade_game_->p_pos);
  output();

  return HuntPreyOutput(1, 1);
}

void Prey::preyAddPreferOnHunterDir(int h_dir_x, int h_dir_y, Pos hunter, Pos prey) {
  // Check whethter hunter's dirction is toward prey or not
  Pos huntToPreyVect;
  huntToPreyVect.set(prey.x - hunter.x, prey.y - hunter.y);
  // Cross product of huntToPreyVect and hunter's direction, if > 0,
  // hunter moving towards prey, else away from prey
  int crossProd = huntToPreyVect.x * h_dir_x + huntToPreyVect.y * h_dir_y;
  if (crossProd > 0) {  // prey prefer to move Counter to hunter's dir
    addPreferNearPrey(prey, -h_dir_x, -h_dir_y);
  } else {  // prey prefer to move along hunter's dir
    addPreferNearPrey(prey, h_dir_x, h_dir_y);
  }
}

void Prey::addPreferNearPrey(Pos prey, int p_dir_x, int p_dir_y) {
  int tmp;

}

void Prey::preyAddDangerOnHunterPredPath(int stepsWent,
    const vector<Pos>& hFutureSteps) {
  for (int i = 0; i < stepsWent; ++i) {
    Pos tmppos = hFutureSteps[i];
    // Adding Danger in capture zone(-2, +2)
    for (int j = tmppos.x -3; j <= tmppos.x + 3; ++j) {
      for (int k = tmppos.y - 3; k <= tmppos.y + 3; ++k) {
        if (j >= 0 && j <= 500 && k >= 0 && k <= 500) {
          dangerMap[j][k] += (InitHunterPredPathDanger - 1.5 * (double)i);
        }
      }
    }
  }
}

