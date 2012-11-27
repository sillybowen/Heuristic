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
  BettingInfo(int id)
  : gambleId(id){ }
};

class Portfolio_jinil{
 public:
  Portfolio_jinil();
  ~Portfolio_jinil(){ }
  void setMode(int mode_);
  void ParseFile(string inFile_srv_);
  vector<double> *makeDecision();
  void getResult();
  void init();                   // step 0
  void setupClassType();         // step 1
  void changeProbOnClass();      // step 2
  void changeProbOnLink();       // step 3
  void selectGambles();          // step 4
  void giveRoundInfo(vector<int>& info);
  int getNumOfStocks() const {
    return gambles_.size();
  }
 private:
  int roundNum;
  int mode;
  vector<Gamble*> gambles_;
  vector<Gamble*> t_gambles_;
  vector<Link*>   links_;
  vector<ResultInfo*> resultInfo_;
  vector<BettingInfo*> bettingInfo_; // selected betting gamble list
  ClassInfo classInfo[16];
  vector<double> decision_;
};

#endif
