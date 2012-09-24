#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "matching.h"
#include "matrix_graph.h"
#include "nn_bowen.h"
#include "evaluate.h"
#include "NN.h"
using namespace std;

#define MAXNUMOFPOINTS 1000

vector<point> p;
void init(const char* filename) {
  ifstream inputfile;
  // Don't add default "point"s if there are NOT so many cities
  p.reserve(MAXNUMOFPOINTS);
  inputfile.open(filename);
  for (int i = 0; i<MAXNUMOFPOINTS; i++) {
    int x;
    point aCity;
    if(!(inputfile>>aCity.id_at_input>>aCity.x>>aCity.y>>aCity.z))
      break;
    aCity.id_at_main = i;
    p.push_back(aCity);
  }

  cout << "p.size() = " << p.size() << endl;
  //  for (int i = 0; i < p.size(); ++i) {
  //    cout << "p element = " << p[i].id_at_input << " " << p[i].x
  //      << " " << p[i].y << " " << p[i].z << endl;
  //  }
  inputfile.close();
}

void work(int startV) {
  Mst m_(p);
  m_.work();
  Matching ma_(m_.giveOddDegreePoints());
  ma_.work2();
  m_.outCost();
  ma_.outCost();
  vector<edge> edgesSet1 = m_.givenAnsEdges();
  vector<edge> edgesSet2 = ma_.givenAnsEdges();
  //  m_.output();
  //  ma_.output();
  cout << "Output MST size: " << (int) edgesSet1.size() << endl;
  cout << "Output matching size: " << (int) edgesSet2.size() << endl;

  int numOfTotalEdges = (int) p.size();
  MatrixGraph myEuler(numOfTotalEdges, edgesSet1, edgesSet2);
  // myEuler.dumpEulerDFSGraph();
  // Testing, make sure myEuler is an Eulerian Graph right now
  myEuler.ifEulerianGraph();
  list<int>* pEulerCircuit = myEuler.findEulerCircuit(startV);
  cout << endl;
  myEuler.trimEulerCircuitToTSP(pEulerCircuit);
  cout << "\nTotal Distance: " << myEuler.sumTSPDistance(pEulerCircuit, p) << endl;


  Evaluate e(p, startV);
  double ans;
  ans = e.evaluate(myEuler.giveResult());
  cout<< "Euler Circuit Greedy Trimming: " << ans<<endl;

  delete pEulerCircuit;

  NN_Bowen nb(p);
  nb.work();
  ans = e.evaluate(nb.giveResult());
  cout<< "NN Greedy: " << ans<<endl;

  NN nj(p);
  nj.work();
  double ans_nj = e.evaluate(nj.give_result());
  cout << "NN Gready (Jinil): "<< ans_nj << endl;
}

void outit() {
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cout << "Please enter Euler Circuit Test input file and StartVertex!\n";
    return 1;
  }

  init(argv[1]);
  work(atoi(argv[2]));
  outit();
}
