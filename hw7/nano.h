#ifndef nano_
#define nano_
#include<vector>
#include <string>
#include <map>
using namespace std;
class Nano {
 private:
  static map<string,int> dirMap_;
  std::vector<int> seq_;
  bool live_;
  int id_;
  int belong_;
  int lastDir_;
 public:
  Nano (int id,string lastDir_,vector<string> seq, bool live, int team);
  static void initializeMap () ;
  static int lookFor(string);
  void output();

};
#endif
