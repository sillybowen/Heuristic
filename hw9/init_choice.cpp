#include <cassert>
#include <cstring>
#include "init_choice.h"
using std::pair;

InitChoice::InitChoice(int numGambles, int numLinks, const vector<Gamble*>& gambles,
    const vector<Link*>& links) : n_gambles_(numGambles),
                                  n_links_(numLinks),
                                  g_data_(&gambles),
                                  l_data_(&links) {
  assert(n_gambles_ == (int)(g_data_->size()));
  // Initialize matrix graph for gamble links
  link_matr_ = new int*[n_gambles_];
  for (int i = 0; i < n_gambles_; ++i) {
    link_matr_[i] = new int[n_gambles_];
    memset(link_matr_[i], 0, sizeof(int) * n_gambles_);
  }

  exp_g_.resize(n_gambles_);
  var_g_.resize(n_gambles_);
  calExpVarsOfGambles();
  initLinksMatrGraph();
}

InitChoice::~InitChoice() {
  for (int i = 0; i < n_gambles_; ++i)
    delete [] link_matr_[i];
  delete [] link_matr_;
}

void InitChoice::calExpVarsOfGambles() {
  for (int i = 0; i < n_gambles_; ++i) {
    // Add class belonging first
    cla_map_.insert(pair<int, int>(g_data_->at(i)->gamble_id, g_data_->at(i)->class_id));

    int gambleId = g_data_->at(i)->gamble_id;
    exp_g_[gambleId] = g_data_->at(i)->h_ret * g_data_->at(i)->h_prob
      + g_data_->at(i)->m_ret * g_data_->at(i)->m_prob
      + g_data_->at(i)->l_ret * g_data_->at(i)->l_prob;

    var_g_[gambleId] =
      g_data_->at(i)->h_prob * (g_data_->at(i)->h_ret - exp_g_[gambleId]) *
        (g_data_->at(i)->h_ret - exp_g_[gambleId])
    + g_data_->at(i)->m_prob * (g_data_->at(i)->m_ret - exp_g_[gambleId]) *
        (g_data_->at(i)->m_ret - exp_g_[gambleId])
    + g_data_->at(i)->l_prob * (g_data_->at(i)->l_ret - exp_g_[gambleId]) *
        (g_data_->at(i)->l_ret - exp_g_[gambleId]);
  }
}

void InitChoice::initLinksMatrGraph() {
  assert(n_links_ == (int)(l_data_->size()));
  for (int i = 0; i < n_links_; ++i) {
    link_matr_[l_data_->at(i)->gi][l_data_->at(i)->gj] = 1;
    link_matr_[l_data_->at(i)->gj][l_data_->at(i)->gi] = 1;
  }
}
