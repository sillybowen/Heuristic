#include "genetic.h"
#include <iostream>

#include "evaluate.h"

using namespace std;

Genetic::Genetic(vector<int> pre_result, vector<point> p) {
  points.resize(pre_result.size());
  for(int i=0; i<pre_result.size(); i++){
    points[i] = p[pre_result[i]];
  }
}

double Genetic::calDistance(vector<point> p, point pre_p, point post_p){
  double distance = pre_p.dis(p[0]);
  for(int i=0; i<p.size()-1; i++){
    distance += p[i].dis(p[i+1]);
  }
  distance += post_p.dis( p[p.size()-1] );
  return distance;
}

int Genetic::calFactorial(int n){
  int result = 1;
  while(n>1){
    result *= n;
    n--;
  }
  return result;
}

// Genetic Algorithm
void Genetic::work(int max_length) {
  double min_distance, original_distance;

  vector< vector<int> > pre_permutation;
  pre_permutation.resize(1);
  pre_permutation[0].push_back(0);

  for(int length=2; length<=max_length; length++){

    cout << "length : " << length << endl;

    // Make permutation
    vector< vector<int> > cur_permutation;
    cur_permutation.resize( calFactorial(length) );
    for(int i=0; i<pre_permutation.size(); i++){
      int index = 0;

      for(int k=0; k<length; k++){
	cur_permutation[i*length + k].resize(length);

	cur_permutation[i*length + k][index] = length-1;
	index++;

	int cur=0;
	for(int t=0; t<pre_permutation[0].size()+1; t++){
	  if(cur_permutation[i*length + k][t] != length-1){
	     cur_permutation[i*length + k][t] = pre_permutation[i][cur];
	     cur++;
	  }
	}
      }
    }

    // Select best case - Generic selection
    for(int start_p=1; start_p<points.size()-max_length-1; start_p++){

      // original distance
      vector<point> min_points;
      point pre_point, post_point;
      min_points.resize(length);
      int index = 0;
      for(int i=start_p; i<length+start_p; i++){
	min_points[index] = points[i];
	index++;
      }
      pre_point = points[start_p - 1];
      post_point = points[start_p + length];
      original_distance = calDistance(min_points, pre_point, post_point);
      min_distance = original_distance;

      // use current permutation : cur_permutation[cp]
      for(int cp=0; cp<cur_permutation.size(); cp++){
	
	vector<point> temp_points;
	temp_points.resize(length);

	for(int q=0; q<length; q++)
	  temp_points[q] = points[start_p + cur_permutation[cp][q]];

	// calculation
	double temp_distance;
	temp_distance = calDistance(temp_points, pre_point, post_point);

	// compare previous distance      
	if(temp_distance < min_distance){

	  // display current result
	  //  cout << min_distance << " -> " << temp_distance << endl;

	  min_distance = temp_distance;
	  for(int t=0; t<min_points.size(); t++){
	    min_points[t] = temp_points[t];
	  }
	}
      }

      // compare original one.
      index = 0;
      if(min_distance < original_distance){   // change this points array
	for(int i=start_p; i<start_p + length; i++){
	  points[i] = min_points[index];
	  index++;
	}
      }
    }

    // Save current permutation -> pre permutation
    pre_permutation.clear();
    pre_permutation.resize( cur_permutation.size() );
    for(int i=0; i<pre_permutation.size(); i++){
      pre_permutation[i].resize( cur_permutation[i].size() );
    }
    for(int i=0; i<cur_permutation.size(); i++){
      for(int j=0; j<cur_permutation[i].size(); j++){
	pre_permutation[i][j] = cur_permutation[i][j];
      }
    }
  }
  
  // Save the result into ansSeq vector
  ansSeq.clear();
  ansSeq.resize(points.size());
  for(int i=0; i<points.size(); i++)
    ansSeq[i] = points[i].id_at_main;

}

// 2-Opt Algorithm
void Genetic::work2(int start_length, int end_length){
  point pre_p, post_p, ch_p[2];
  double original_sum, reverse_sum;
  for(int interval=start_length; interval<=end_length; interval++){

    for(int start_p=1; start_p<points.size()-interval-1; start_p++){
      pre_p = points[ start_p-1 ];
      post_p = points[ start_p + interval ];
      ch_p[0] = points[ start_p ];
      ch_p[1] = points[ start_p + interval - 1 ];

      original_sum = pre_p.dis(ch_p[0]) + post_p.dis(ch_p[1]);
      reverse_sum = pre_p.dis(ch_p[1]) + post_p.dis(ch_p[0]);

      // If the reversed interval is better,
      // Update the result
      if(reverse_sum < original_sum){
	int index = 0;
	int count = (int)(interval / 2);
	point temp;

	while(count>0){
	  temp = points[start_p + count - 1];
	  points[start_p + count - 1] = points[start_p + interval - count];
	  points[start_p + interval - count] = temp;
	  count--;
	}
      }
    }
  }
  // Save the result into ansSeq vector
  ansSeq.clear();
  ansSeq.resize(points.size());
  for(int i=0; i<points.size(); i++)
    ansSeq[i] = points[i].id_at_main;

}

void Genetic::work3(int start_length, int end_length){

  point ch_1[2], ch_2[2], pre_p[2], post_p[2];
  for(int length=start_length; length<=end_length; length++){
    
    // i : first array start point
    for(int i=1; i<points.size()-2*length; i++){
      ch_1[0] = points[i];
      ch_1[1] = points[i+length-1];
      pre_p[0] = points[i-1];
      post_p[0] = points[i+length];
  
      // j : second array start point
      for(int j=i+length+1; j<points.size()-length; j++){
	ch_2[0] = points[j];
	ch_2[1] = points[j+length-1];
	pre_p[1] = points[j-1];
	post_p[1] = points[j+length];

	double original_sum = pre_p[0].dis( ch_1[0] )
	  + post_p[0].dis( ch_1[1] ) 
	  + pre_p[1].dis( ch_2[0] )
	  + post_p[1].dis( ch_2[1] );

	double mute_sum = pre_p[0].dis( ch_2[0] )
	  + pre_p[1].dis( ch_1[0] )
	  + post_p[1].dis( ch_1[1] )
	  + post_p[0].dis( ch_2[1] );

	if(mute_sum < original_sum){
       	  cout << "changed " << endl;
	  vector<point> temp;
	  temp.resize(length);
	  for(int k=0; k<length; k++){
	    temp[k] = points[i+k];
	    points[i+k] = points[j+k];
	    points[j+k] = temp[k];
	  }
	}
      }
    }
  }

  // Save the result into ansSeq vector
  ansSeq.clear();
  ansSeq.resize(points.size());
  for(int i=0; i<points.size(); i++)
    ansSeq[i] = points[i].id_at_main;
}

vector<int> Genetic::giveResult() {
  return ansSeq;
}

