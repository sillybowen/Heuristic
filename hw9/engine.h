#ifndef engine_h_
#define engine_h_
#include<string>
#include<vector>
#include "stock.h"
using namespace std;
class Engine {
 private:
  vector<Stock*> stocks_;
  vector<double> decision_;
 public:
  Engine(){};
  ~Engine();
  void ParseFile(string filePath);
  void giveRoundInfo(vector<int>& info);
  void output();
  vector<double> *makeDecision();

  int getNumOfStocks() const {
    return stocks_.size();
  }
};

#endif
