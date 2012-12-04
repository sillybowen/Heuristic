#ifndef gamble_h
#define gamble_h

struct Link {
  int gi;
  int gj;
  Link(int gi_, int gj_)
  :gi(gi_), gj(gj_){ }
};

class Gamble {
 private:
  
 public:
  int gamble_id;
  int class_id;
  double h_ret, m_ret, l_ret;
  double h_prob, m_prob, l_prob;
  double expect_ret;    // expectional return value
  double previous_ret;

  Gamble(int gamble_id_, int class_id_, float h_ret_, float h_prob_,
	 float m_ret_, float m_prob_, float l_ret_, float l_prob_);
  ~Gamble() { }
  void calculateExpectRet();
  void increaseHighProb();   // Increase h_prob, decrease l_prob
  void increaseLowProb();    // Increase l_prob, decrease h_prob
  friend class InitChoice;
  void updatePreviousRet(int type);
};

#endif
