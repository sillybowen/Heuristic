#include "NN_gready.h"
#include "basic_geo.h"

NN::NN(vector<point> p){
  points.resize(p.size());
  for(int i=0; i<p.size(); i++){
    points[i].p = p[i];
    if(i==0){
      points[i].conn_p[0] = 0;
      points[i].conn_p[1] = -1;
    }else{
      points[i].conn_p[0] = -1;
      points[i].conn_p[1] = -1;
    }
  }
};

void NN::createEdges() {
  for (int i = 0; i<points.size()-1;i++)
    for (int j = i+1; j<points.size();j++) {
      edge e(i,j, points[i].p.dis(points[j].p));
      edges.push_back(e);
    }
  sort(edges.begin(),edges.end(),comparee);
}

void NN::findNN(){
  int nodes = points.size();

  for(int index=0; index<edges.size(); index++){

    if(points[edges[index].p[0]].conn_empty() && points[edges[index].p[1]].conn_empty()){
	points[edges[index].p[0]].input_conn_p(edges[index].p[1]);
	points[edges[index].p[1]].input_conn_p(edges[index].p[0]);
	ans_edges.push_back(edges[index]);
     }
  }

  /* We don't need to input the last edge (the last city - the first city)

  // the last edge should be came back to the city 1
  for(int i=0; i<ans_edges.size(); i++){
    bool b1=false, b2=false;

    if((b1 = points[ans_edges[i].p[0]].conn_empty()) ||
       (b2 = points[ans_edges[i].p[1]].conn_empty())){
      
      point p;
      if(b1){
	points[ans_edges[i].p[0]].input_conn_p(0);
	p = points[ans_edges[i].p[0]].p;
      }else{
	points[ans_edges[i].p[1]].input_conn_p(0);
	p = points[ans_edges[i].p[1]].p;
      }

      for(int j=0; j<edges.size(); j++){
	if((edges[j].p[0] == 0 && edges[j].p[1] == p.id_at_main) ||
	   (edges[j].p[0] == p.id_at_main && edges[j].p[1] == 0)){
	  ans_edges.push_back(edges[j]);
	  break;
	}
      }
      break;
    }
  }
  */
}

void NN::work(){
  createEdges();
  findNN();
}

vector<point> NN::give_result(){
  ans_sequence.push_back(points[0].p);
  int search_p = 0;
  while(ans_edges.size()>0){
    point p;
    for(int index=0; index<ans_edges.size(); index++){
      if(ans_edges[index].p[0] == search_p || ans_edges[index].p[1] == search_p){

	if(ans_edges[index].p[0] == search_p){
	  p = points[ans_edges[index].p[0]].p;
	  search_p = ans_edges[index].p[1];
	}else{
	  p = points[ans_edges[index].p[1]].p;
	  search_p = ans_edges[index].p[0];
	}
	
	ans_edges.erase(ans_edges.begin() + index);
	break;
      }
    }
    ans_sequence.push_back(p);
  }
}
