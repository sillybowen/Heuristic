#include <climits>
#include <cstring>
#include <cassert>
#include <iostream>
#include <iomanip>
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
  int p_dir_x, p_dir_y;

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

  // Choose direction according to dangerMap
  lookforbestMove(evade_game_->p_pos, p_dir_x, p_dir_y);
  output();
  cout << "Best Move: (" << p_dir_x << ", " << p_dir_y << ")" << endl;
  globalDecay();

  return HuntPreyOutput(p_dir_x, p_dir_y);
}

void Prey::lookforbestMove(Pos prey, int& p_dir_x, int& p_dir_y) const {
  int preyX = prey.x;
  int preyY = prey.y;
  long minDanger = LONG_MAX;
  Pos bestDir;

  for (int i = preyX - 1; i <= preyX + 1; ++i) {
    for (int j = preyY - 1; j <= preyY + 1; ++j) {
      if (dangerMap[i][j] < minDanger) {
        minDanger = dangerMap[i][j];
        bestDir.set(i, j);
      }
    }
  }

  p_dir_x = bestDir.x - preyX;
  p_dir_y = bestDir.y - preyY;

  assert(abs(p_dir_x) == 1);
  assert(abs(p_dir_y) == 1);
}

void Prey::preyAddPreferOnHunterDir(int h_dir_x, int h_dir_y, Pos hunter, Pos prey) {
  assert(abs(h_dir_x) == 1);
  assert(abs(h_dir_y) == 1);

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

void Prey::globalDecay() {
  for (int i = 0; i < 500; ++i) {
    for (int j = 0; j < 500; ++j) {
      dangerMap[i][j] /= GlobalDecayRatio;
    }
  }
}

void Prey::addPreferNearPrey(Pos prey, int p_dir_x, int p_dir_y) {
  int tmp;
  Pos perpendUp, perpendDown;
  perpendUp.set(p_dir_x, -p_dir_y);
  perpendDown.set(-p_dir_x, p_dir_y);

  for (int dirIndex = -PreyPreferDiagonalRange; dirIndex <= PreyPreferDiagonalRange;
      ++ dirIndex) {
    if (dirIndex == 0)
      continue;

    Pos middle;
    middle.set(prey.x + dirIndex * p_dir_x, prey.y + dirIndex * p_dir_y);
    int value = PreyPreferDirMaxPreference / dirIndex;
    // Set middle value
    if (middle.x >= 0 && middle.x <= 500 && middle.y >= 0 && middle.y <= 500)
      dangerMap[middle.x][middle.y] += (2 * value);

    for (int perpendInd = 1; perpendInd <= PreyPreferPerpendicularRange;
        ++perpendInd) {
      int x = middle.x + perpendInd * perpendUp.x;
      int y = middle.y + perpendInd * perpendUp.y;
      if (x >= 0 && x <= 500 && y >= 0 && y <= 500) {
        dangerMap[x][y] += (value / (perpendInd * perpendInd));
      }
      // perpendDown dirction
      x = middle.x + perpendInd * perpendDown.x;
      y = middle.y + perpendInd * perpendDown.y;
      if (x >= 0 && x <= 500 && y >= 0 && y <= 500) {
        dangerMap[x][y] += (value / (perpendInd * perpendInd));
      }
    }
  }
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

