#include <cstring>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "evasion.h"

using std::stringstream;
using std::setw;

Evasion::Evasion(Moveable* moveable, int n, int m, int srv_port)
    : N_(n), M_(m), my_obj_(moveable), arch_clt_(registerSrv(srv_port)) {
  my_obj_->setEvadeGamePtr(this);
  // Insert game borders as special walls (No thickness)
  hor_walls_.push_back(new Moveable::Wall(0, 0, 500, 0, 0));
  hor_walls_.push_back(new Moveable::Wall(0, 500, 500, 500, 2));
  ver_walls_.push_back(new Moveable::Wall(0, 0, 0, 500, 1));
  ver_walls_.push_back(new Moveable::Wall(500, 0, 500, 500, 3));
  // matrix graph showing adjacency between walls
  adj_walls_ = new int*[M_ + 4];
  for (int i = 0; i < M_ + 4; ++i) {
    adj_walls_[i] = new int[M_ + 4];
    memset(adj_walls_[i], 0, sizeof(int) * (M_ + 4));
  }
  // Set init adjacency between game borders
  for (int i = 1; i < 4; ++i) {
    adj_walls_[i - 1][i] = 1;
    adj_walls_[i][i - 1] = 1;
    adj_walls_[i][(i + 1) % 4] = 1;
    adj_walls_[(i + 1) % 4][i] = 1;
  }
  wall_index = 4;
}

Evasion::~Evasion() {
  delete my_obj_;
  delete arch_clt_;
  hor_walls_.clear();
  ver_walls_.clear();
  for (int i = 0; i < M_ + 4; ++i)
    delete [] adj_walls_[i];
  delete [] adj_walls_;
}

int Evasion::startGame(string& teamName) {
  int gameResult = playing;
  string fromSrv, fromPly;
  teamName.push_back('\n');
  
  try {
    (*arch_clt_) << teamName;

    do {
      (*arch_clt_) >> fromSrv;
      if (fromSrv.empty() || fromSrv.compare("Bye") == 0)
        break;

      my_obj_->tryMove();

   
    } while (1);
  } catch (SocketException& se) {
    assert(false);
  }

  return gameResult;
}


void Evasion::readSrvUpdateStates(const string& fromSrv) {
  stringstream ss;
  ss << fromSrv;
  // Initialization ; because walls can be created/removed.
  hor_walls_.clear();
  ver_walls_.clear();
  hor_walls_.push_back(new Moveable::Wall(0, 0, 500, 0, 0));
  hor_walls_.push_back(new Moveable::Wall(0, 500, 500, 500, 2));
  ver_walls_.push_back(new Moveable::Wall(0, 0, 0, 500, 1));
  ver_walls_.push_back(new Moveable::Wall(500, 0, 500, 500, 3));

  Moveable::Wall *wall;
  string temp;
  while(!ss.eof()){
    ss >> temp;
    if(temp == "H"){
      ss >> h_pos.x;
      ss >> h_pos.y;
    }else if(temp == "P"){
      ss >> p_pos.x;
      ss >> p_pos.y;
    }else if(temp == "W" || temp == ","){
      while(true){
	int x1, y1, x2, y2;
	ss >> x1;
	ss >> y1;
	ss >> x2;
	ss >> y2;
	if(y1 == y2){
	  hor_walls_.push_back(new Moveable::Wall(x1, y1, x2, y2, wall_index++));
	}else if(x1 == x2){
	  ver_walls_.push_back(new Moveable::Wall(x1, y1, x2, y2, wall_index++));
	}
	if(ss.get() != ',')
	  break;
      }
    }
  }
  /*
  int tmp, wt, gamePos, ind = 0;
  while ((tmp = ss.get()) != '|');

  while (ss.peek() != '|') {
    ss >> wt;
    assert(ss.get() == int(','));
    ss >> gamePos;

    ++ind;
  }
  */
}

// DFS adj_walls_ matrix graph, find all undirected circles
void Evasion::findWallCircles() {
}

// Register with the architecture server
ClientSocket* Evasion::registerSrv(int srv_port) {
  ClientSocket* pClt_socket = NULL;

  try {
    pClt_socket = new ClientSocket("localhost", srv_port);
  } catch (SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << " PortNum: "
      << srv_port << std::endl;
    assert(0);
  }

  return pClt_socket;
}

void Evasion::dumpAdjWallsMatrixGraph() const {
  for (int i = 0; i < M_ + 4; ++i) {
    for (int j = 0; j < M_ + 4; ++j) {
      std::cout << setw(3) << adj_walls_[i][j];
    }
    std::cout << std::endl;
  }
}
