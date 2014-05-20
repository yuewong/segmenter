// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_STATE_TRANS_MATRIX_H_
#define UTIL_SEGMENTER_PUBLIC_STATE_TRANS_MATRIX_H_

#include <string>

#include "base/basictypes.h"
#include "util/segmenter/base/states.h"

namespace util {
namespace segmenter {
class StateTransMatrix {
 public:
  StateTransMatrix();
  ~StateTransMatrix() {}

 public:
  bool LoadTransProb(const std::string& file);
  float GetTransProb(State a, State b) const;

 private:
  bool ParserStateTrans(const std::string& line, State* a, State* b, float* freq);

 private:
  const static char kDefaultSplitSymbol = ':';
  const static int32 kDefaultRowNum = 3;
  const static int32 kStateNum = 4;
  const static int32 STATE_A = 0;
  const static int32 STATE_B = 1;
  const static int32 PROB = 2;

 private:
  float state_trans_matrix_[kStateNum][kStateNum];

 private:
  DISALLOW_COPY_AND_ASSIGN(StateTransMatrix);
};
}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_PUBLIC_STATE_TRANS_MATRIX_H_`
