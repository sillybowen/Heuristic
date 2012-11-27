#ifndef portfolio_jinil_h
#define portfolio_jinil_h
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include "gamble.h"
using namespace std;

struct ClassInfo{
  vector<int> member_list;
  int type; // favorable:+1, neutral:0, unfavorable:-1, undefined:-2
  ClassInfo()
  :type(-2){ }
};

struct ResultInfo{
  int gambleId;
  int type; // favorable:+1, neutral:0, unfavorable:-1
  ResultInfo(int gambleId_, int type_)
  :gambleId(gambleId_), type(type_){ }
};

struct BettingInfo{
  int gambleId;
  float fraction;
  BettingInfo(int id, float fract)
  : gambleId(id), fraction(fract){ }
};

class Portfolio_jinil{
 public:
  Portfolio_jinil(vector<Gamble*> gambles, vector<Link*> links, int alloc);
  ~Portfolio_jinil(){ }
  vector<double> betting();
  void readResultFromSrv(string result);
  vector<double> getResult();

 private:
  vector<Gamble*> gambles_;
  vector<Link*>   links_;
  vector<ResultInfo*> resultInfo_;
  vector<BettingInfo*> bettingInfo_;
  vector<ClassInfo*> classInfo_;
  float allocation;
  ClassInfo classInfo[16];
};

#endif
