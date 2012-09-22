#ifndef basic_geo_h
#define basic_geo_h
#include <cmath>
#include <cstring>
struct point {
  int id_at_input;
  int id_at_main;
  double x,y,z;

  void set(int i,int i2,double xx,double yy,double zz) {
    id_at_input = i;
    id_at_main = i2;
    x = xx;
    y = yy;
    z = zz;
  };

  double dis (point p) {
    return sqrt((x-p.x)*(x-p.x) +
                (y-p.y)*(y-p.y) +
                (z-p.z)*(z-p.z));
  };
};

struct edge {
  int p[2];
  double dis;
  edge(int x,int y, double d) {
    p[0] = x;
    p[1] = y;
    dis = d;
  };
};

static bool comparee(edge e1,edge e2) {
  return e1.dis<e2.dis;
};

#endif
