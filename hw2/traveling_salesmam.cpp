#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "matching.h"
#include "matrix_graph.h"
#include "nn_bowen.h"
#include "evaluate.h"
#include "NN.h"
#include "trim.h"
#include "genetic.h"
using namespace std;

#define MAXNUMOFPOINTS 1000
const char* outFileName = "tsp_output.txt";

vector<point> p;
vector<int> ansV;
vector<double> distanc;

void init() {
  ifstream inputfile;
  // Don't add default "point"s if there are NOT so many cities
  p.reserve(MAXNUMOFPOINTS);
  inputfile.open("tsp_input.txt");
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
  for (int i = 0; i<p.size();i++)
    for (int j = 0;j<p.size();j++)
      distanc.push_back(p[i].dis(p[j]));
}

void work() {
  int startV = 0;

  Mst m_(p);
  m_.work();
  Matching ma_(m_.giveOddDegreePoints());
  ma_.work2();
  m_.outCost();
  ma_.outCost();
  vector<edge> edgesSet1 = m_.givenAnsEdges();
  vector<edge> edgesSet2 = ma_.givenAnsEdges();
  cout << "Output MST size: " << (int) edgesSet1.size() << endl;
  cout << "Output matching size: " << (int) edgesSet2.size() << endl;

  int numOfTotalEdges = (int) p.size();
  MatrixGraph myEuler(numOfTotalEdges, edgesSet1, edgesSet2);

  myEuler.ifEulerianGraph();
  list<int>* pEulerCircuit = myEuler.findEulerCircuit(startV);

  Trim trim (p,pEulerCircuit,&distanc);
  Trim trimWithRevert (p,pEulerCircuit,&distanc);
  myEuler.trimEulerCircuitToTSP(pEulerCircuit);

  Evaluate e(p, startV);
  double ansC;
  vector<int> tmpAns;
  double ans;


  NN_Bowen nb(p);
  nb.work();
  tmpAns = nb.giveResult();
  ans = e.evaluate(tmpAns);
  ansV = tmpAns;
  ansC = ans;
  cout<< "NN Greedy: " << ans<<endl;


  tmpAns = myEuler.giveResult();
  ans = e.evaluate(tmpAns);
  if (ans<ansC&&ans>0) {
    ansC = ans;
    ansV = tmpAns;
  }
  cout<< "Euler Circuit Greedy Trimming: " << ans<<endl;
  //  myEuler.printOutputTSPTrip(pEulerCircuit, outFileName);
  delete pEulerCircuit;


  trim.work();
  tmpAns = trim.giveResult();
  ans = e.evaluate(tmpAns);
  if (ans<ansC&&ans>0) {
    ansC = ans;
    ansV = tmpAns;
  }
  cout<<"Bowen's trim: "<< ans<<endl;


  trimWithRevert.work2();
  tmpAns = trimWithRevert.giveResult();
  ans = e.evaluate(tmpAns);
  if (ans<ansC&&ans>0) {
    ansC = ans;
    ansV = tmpAns;
  }
  cout<<"Trim with revert: "<< ans<<endl;

  tmpAns = trim.work3(ansV);
    ans = e.evaluate(tmpAns);
  if (ans<ansC&&ans>0) {
    ansC = ans;
    ansV = tmpAns;
  }

  cout<<"optimization: "<< ans<<endl;
  tmpAns = trim.work4(ansV);
  ans = e.evaluate(tmpAns);
  if (ans<ansC&&ans>0) {
    ansC = ans;
    ansV = tmpAns;
  }
  cout<<"optimization2: "<< ans<<endl;


  // Genetic
  cout << "Start Genetic Algorithm ... " << endl;
  Genetic ge(tmpAns, p);
  int max_length = 8;
  ge.work(max_length);
  ge.work2(++max_length, MAXNUMOFPOINTS-2);
  tmpAns = ge.giveResult();

  ans = e.evaluate(tmpAns);
  if (ans < ansC && ans > 0) {
    ansC = ans;
    ansV = tmpAns;
  }
  cout << "Genetic : " << ans << endl;
  cout<<"Best is"<<ansC<<endl;

}

void outit() {
  ofstream outF;
  Evaluate e(p,0);
  cout<<e.evaluate(ansV)<<endl;
  outF.open(outFileName);
  for (int i = 0; i<ansV.size();i++)
    outF<<ansV[i]+1<<endl;
  outF.close();

}

int main(int argc, char* argv[]) {

  init();
  work();
  outit();
}
