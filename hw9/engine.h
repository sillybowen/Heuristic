#ifndef engine_h_
#define engine_h_
#include<string>
#include<vector>
#include "stock.h"
using namespace std;
class Engine {
 private:
  vector<Stock*> stocks;
 public:
  Engine(){};
  ~Engine();
  void ParseFile(string filePath);
  void GetRoundInfo(vector<int> info);
};

#endif
