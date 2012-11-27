#ifndef gamble_h
#define gamble_h

struct Link{
  int gi;
  int gj;
  Link(int gi_, int gj_)
  :gi(gi_), gj(gj_){ }
};

class Gamble{
 private:
  int gamble_id;
  int class_id;
  float h_ret, m_ret, l_ret;
  float h_prob, m_prob, l_prob;

 public:
  Gamble(int gamble_id_, int class_id_, float h_ret_, float h_prob_,
	 float m_ret_, float m_prob_, float l_ret_, float l_prob_);
  ~Gamble() { }
};

#endif
