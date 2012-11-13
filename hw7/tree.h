#ifndef tree_
#define tree_
#include <vector>
#include <string>
#include <sstream>
using namespace std;

struct tree {
  string data;
  vector<tree> children;
  tree() {}
  tree(const string& inputData) : data(inputData) {}

  tree(vector<char>::const_iterator& iter) {
    skip_blank(iter);
    if (*iter != '(') {
      data = read_word(iter);
      return;
    }
    ++iter;
    for (;;) {
      skip_blank(iter);
      if (*iter == ')') break;
      children.push_back(iter);
    }
    ++iter;
  }
  tree const& operator[](size_t idx) const {
    return children[idx];
  }
  bool operator==(string const& s) const {
    return data == s;
  }

private:
  static void skip_blank(vector<char>::const_iterator& iter) {
    while (*iter <= ' ') ++iter;
  }
  static string read_word(vector<char>::const_iterator& iter) {
    vector<char>::const_iterator begin = iter;
    while (*iter != ')' && *iter != '(' && *iter > ' ') ++iter;
    return string(begin, iter);
  }

};

inline ostream& operator<<(ostream& os, tree const& t) {
  if (!t.data.empty()) {
    os << t.data;
    return os;
  }
  os << '(';
  char const* separator = "";
  vector<tree>::const_iterator childIter = t.children.begin();
  while (childIter != t.children.end()) {
    os << separator << *childIter;
    separator = " ";
    ++childIter;
  }
  os << ')';
  return os;
}

template<typename T> inline string to_string(T const& t) {
  stringstream ss;
  ss << t;
  return ss.str();
}

template<typename T> inline int to_int(T const& t) {
  stringstream ss;
  ss << t;
  int x;
  ss >> x;
  return x;
}
#endif
