#ifndef parser_
#define parser_
#include"tree.h"
#include"loc.h"
class Parser {
 private:
  vector<Location*> locs;
 public:
  void  GetNodes(tree * srvTr);
  void  GetEdges(tree * srvTr);
  void  output();
};
#endif
