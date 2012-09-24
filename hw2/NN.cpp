#include "NN.h"
#include "basic_geo.h"
#include "limits.h"

using namespace std;

NN::NN(vector<point> p){
  points.resize(p.size());
  for(int i=0; i<p.size(); i++){
    points[i] = p[i];
  }
};

void NN::createEdges() {
  for (int i = 0; i<points.size()-1;i++)
    for (int j = i+1; j<points.size();j++) {
      edge e(i,j, points[i].dis(points[j]));
      edges.push_back(e);
    }
  sort(edges.begin(),edges.end(),comparee);
}

void NN::findPath(){
 
  ans_sequence.push_back(0);
  while(ans_sequence.size() < 1000){
    int curr_city = ans_sequence.back();
    int next_city;
    int minimum_path = UINT_MAX;
    for(int i=0; i<edges.size(); i++){
      if(edges[i].p[0] == curr_city || edges[i].p[1] == curr_city){

	// Get possible next city
	if(edges[i].p[0] == curr_city)
	  next_city = edges[i].p[1];
	else
	  next_city = edges[i].p[0];
	
	// Check this edge can be circuit.
	// Check the ans_sequence if the next_city already visited or not.
	bool include_f = false;
	for(int j=0; j<ans_sequence.size(); j++){
	  if(ans_sequence[j] == next_city){
	    include_f = true;
	    break;
	  }
	}
	// If this city is not visited yet, push it.
	if(!include_f)
	  ans_sequence.push_back(next_city);
      }
    }
  }
}

void NN::work(){
  createEdges();
  findPath();
}

vector<int> NN::give_result(){
  return ans_sequence;
}
