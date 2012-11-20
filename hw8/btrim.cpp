#include "btrim.hpp"
using namespace std;

void ltrim(string& str, const locale& loc) {
  string::size_type pos = 0;
  while (pos < str.size() && isspace(str[pos], loc)) pos++;
  str.erase(0, pos);
}

void rtrim(string& str, const locale& loc) {
  string::size_type pos = str.size();
  while (pos > 0 && isspace(str[pos - 1], loc)) pos--;
  str.erase(pos);
}

void btrim(string& str, const locale& loc) {
  ltrim(str, loc);
  rtrim(str, loc);
}
