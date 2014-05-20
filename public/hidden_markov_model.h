// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_HIDDEN_MARKOV_MODEL_H_
#define UTIL_SEGMENTER_PUBLIC_HIDDEN_MARKOV_MODEL_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/scoped_ptr.h"

namespace util {
namespace segmenter {
class BemsEmitMatrix;
class StateTransMatrix;
class StartProbMatrix;

class HiddenMarkovModel {
 public:
  HiddenMarkovModel();
  ~HiddenMarkovModel();

 public:
  bool Init(
      const std::string& start_prob_file,
      const std::string& tran_prob_file,
      const std::string& emit_prob_file);

  void Viterbi(
      const std::vector<int32>& simple_char_code,
      std::pair<int, int>& range,
      std::vector<int>* final_path) const;

 private:
  bool InitStartProb(const std::string& file_path);
  bool InitTranProb(const std::string& file_path);
  bool LoadEmitMatrix(const std::string& file_path);

 private:
  bool inited_;
  scoped_ptr<BemsEmitMatrix> emit_matrix_;
  scoped_ptr<StateTransMatrix> state_trans_matrix_;
  scoped_ptr<StartProbMatrix> start_prob_;

 private:
  DISALLOW_COPY_AND_ASSIGN(HiddenMarkovModel);
};
}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_PUBLIC_HIDDEN_MARKOV_MODEL_H_
