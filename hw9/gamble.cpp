#include "gamble.h"
using namespace std;

Gamble::Gamble(int gamble_id_, int class_id_, float h_ret_, float h_prob_,
	 float m_ret_, float m_prob_, float l_ret_, float l_prob_)
    :gamble_id(gamble_id_), class_id(class_id_), h_ret(h_ret_), h_prob(h_prob_),
    m_ret(m_ret_), m_prob(m_prob_), l_ret(l_ret_), l_prob(l_prob_) 
{ }

