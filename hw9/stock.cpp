#include "stock.h"
#include <iostream>
#define PP 0.5
using namespace std;
void Stock::assignProb(int index, double p,double r) {
  prob_[index] = p;
  ret_[index] = r;
}

void Stock::addNeighbor(int index) {
  neighbor_.push_back(index);
}
void Stock::addRoundInfo(int info) {
  roundInfo_.push_back(info);
}
void Stock::output() {
  cout<<id_<<' '<<classID_<<' ';
  for (int i = 0; i<3; i++) {
    cout<<prob_[i]<<' '<<ret_[i]<<' ';
  }
  cout<<endl;
  cout<<"Neighbours:";
  for (int i=0 ; i<neighbor_.size(); i++)
    cout<<neighbor_[i]<<' ';
  cout<<endl;
  cout<<"Round Info:";
  for (int i = 0; i<roundInfo_.size();i++) {
    if (roundInfo_[i] == 0) 
      cout<<'h';
    else if (roundInfo_[i] == 1)
      cout<<'m';
    else cout<<'l';
    cout<<' ';
  }
  cout<<endl;
}

Stock::Scores Stock::neighborsScore() {
  Scores ret;
  ret.s[0] = 0;
  ret.s[1] = 0;
  ret.s[2] = 0;
  ret.sum = 0;
  Scores tmp;
  for (int i = 0; i<stocks_->size();i++) {
    if (stocks_->at(i)!=this)
      tmp = stocks_->at(i)->pureScore();
    for (int i = 0; i<3; i++)
      ret.s[i]+= tmp.s[i];
    ret.sum+=tmp.sum;
  }
  if (ret.sum!=0)
  for (int i = 0; i<3; i++)
    ret.s[i]/=ret.sum;
  return ret;
}
Stock::Scores Stock::pureScore() {
  Scores s;
  s.s[0] = 0;
  s.s[1] = 0;
  s.s[2] = 0;
  s.sum = 0;
  int start = roundInfo_.size()-20;
  if (start<0) start = 0;

  for (int i= start; i<roundInfo_.size(); i++) {
    s.sum+=(i-start+1);
    s.s[roundInfo_[i]]+= (i-start+1);
  }
  if (s.sum!=0)
    for (int i = 0; i<3; i++) {
      s.s[i]/=s.sum;
      //      cout<<s.s[i]<<' ';
    }
  //  cout<<endl;
  //  cout<<"s sum"<<s.sum<<endl;
  return s;
}
double Stock::score() {
  double sum = 0;
  double ret=0;
  if (roundInfo_.size() == 0) {
    for (int i = 0; i<3; i++)
      ret += (prob_[i]*ret_[i]);
    return ret;
  }
  /*  int start = roundInfo_.size()-20;
  if (start<0) start = 0;

  for (int i= start; i<roundInfo_.size(); i++) {
    sum+=(i-start+1);
    tmp[roundInfo_[i]]+= (i-start+1);
    }*/
  double tmp[3];
  int count;
  double sum1=0;
  tmp[0] = 0;
  tmp[1] = 0;
  tmp[2] = 0;

  Scores s = pureScore();
  Scores s1 = neighborsScore();
  if (s.s[0]<0.1) return 0;
  cout<<"test"<<endl;
  sum1 = 0;

  for (int i = 0; i<3; i++) {
    cout<<"s1 "<<i<<' '<<s1.s[i]<<endl;
    cout<<"s"<<i<<' '<<s.s[i]<<endl;
    s.s[i] = PP*s.s[i]+(1.0-PP)*s1.s[i];
    cout<<"s"<<i<<' '<<s.s[i]<<endl;
    sum1+=s.s[i];
  }
  cout<<sum1<<endl;
  for (int i = 0; i<3; i++) {
    cout<<s.s[i]<<' ';
    ret+= (ret_[i] * s.s[i]/sum1);///s.sum);
  }
  cout<<endl;
  if (ret<2.0) ret = 0;
  cout<<ret<<endl;
  return ret;
}

