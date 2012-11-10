#include "parser.h"
#include <vector>
#include <sstream>
using namespace std;

void Parser::GetNodes(tree * srvTr) {
  locs.resize((*srvTr)[0][1][0][1].children.size());  
  Location * lc;
  stringstream ss;
  int id,x,y;
  for (int i = 0; i<(*srvTr)[0][1][0][1].children.size();i++) {
    cout<<(*srvTr)[0][1][0][1][i]<<endl;
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
    cout<<(*srvTr)[0][1][1][1][i]<<endl;
    istringstream((*srvTr)[0][1][1][1][i][0][1].data)>>n1;
    istringstream((*srvTr)[0][1][1][1][i][1][1].data)>>n2;
    cout<<n1<<' '<<n2<<endl;
    locs[n1]->AddChildren(locs[n2]);
    locs[n2]->AddChildren(locs[n1]);
  }
}
void Parser::output() {
  for (int i = 0; i<locs.size(); i++)
    locs[i]->output();
}
