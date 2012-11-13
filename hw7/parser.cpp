#include "parser.h"
#include <vector>
#include <sstream>
using namespace std;
vector<Location> Parser::work(tree*srvTr) {
  GetNodes(srvTr);
  GetEdges(srvTr);
  GetEaten(srvTr);
  GetNanos(srvTr);
  output();
  return worker.makeDecision(&locs,&nanos);
}
void Parser::GetNodes(tree * srvTr) {
  locs.resize((*srvTr)[0][1][0][1].children.size());  
  Location * lc;
  stringstream ss;
  int id,x,y;
  for (int i = 0; i<(*srvTr)[0][1][0][1].children.size();i++) {
    cerr<<(*srvTr)[0][1][0][1][i]<<endl;
    istringstream((*srvTr)[0][1][0][1][i][0][1].data)>>id;
    istringstream((*srvTr)[0][1][0][1][i][1][1][0][1].data)>>x;
    istringstream((*srvTr)[0][1][0][1][i][1][1][1][1].data)>>y;
    lc = new Location(id,x,y);
    locs[id] = lc;
  }
}
void Parser::GetEdges(tree * srvTr) {

  int n1,n2;
  for (int i = 0; i<(*srvTr)[0][1][1][1].children.size();i++) {
    cerr<<(*srvTr)[0][1][1][1][i]<<endl;
    istringstream((*srvTr)[0][1][1][1][i][0][1].data)>>n1;
    istringstream((*srvTr)[0][1][1][1][i][1][1].data)>>n2;
    locs[n1]->AddChildren(locs[n2]);
    locs[n2]->AddChildren(locs[n1]);
  }
}

void Parser::GetEaten(tree *srvTr) {
  int id,team;
  for (int i = 0; i<(*srvTr)[3][1].children.size();i++) {
    cerr<<(*srvTr)[3][1][i]<<endl;
    istringstream((*srvTr)[3][1][i][0][0][1].data)>>id;
    team = (*srvTr)[3][1][i][1].data[4] -'0';
    locs[id]->setEaten(team);
  }
}

void Parser::GetNanos( tree *srvTr) {
    cerr<<(*srvTr)[2][1]<<endl;
    vector<string> dir;
    string lastDir;
    int id;
    int team;
    Nano * nn;
    nanos.clear();
    dir.resize(4);
    for (int i = 0; i< (*srvTr)[2][1].children.size();i++) {
      cerr<<(*srvTr)[2][1][i]<<endl;
      istringstream((*srvTr)[2][1][i][0][1][0][1].data)>>id;
      lastDir = (*srvTr)[2][1][i][1][1].data;
      for (int j = 0; j<4; j++) {
	dir[j] = (*srvTr)[2][1][i][2][1][j].data;
      }
      team = (*srvTr)[2][1][i][3][1].data[4]-'0';
      nn = new Nano (id,lastDir,dir,false,team);
      nanos.push_back(nn);
    }

    for (int i = 0; i< (*srvTr)[1][1].children.size();i++) {
      cerr<<(*srvTr)[1][1][i]<<endl;
      istringstream((*srvTr)[1][1][i][0][1][0][1].data)>>id;
      lastDir = (*srvTr)[1][1][i][1][1].data;
      for (int j = 0; j<4; j++) {
	dir[j] = (*srvTr)[1][1][i][2][1][j].data;
      }
      team = (*srvTr)[1][1][i][3][1].data[4]-'0';
      nn = new Nano (id,lastDir,dir,true,team);
      nanos.push_back(nn);
    }

    for (int i = 0; i<nanos.size();i++)
      nanos[i]->output();
}

void Parser::output() {
  for (int i = 0; i<locs.size(); i++)
    locs[i]->output();
}
