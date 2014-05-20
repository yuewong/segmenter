// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_START_PROB_MATRIX_H_
#define UTIL_SEGMENTER_PUBLIC_START_PROB_MATRIX_H_

#include <string>

#include "util/segmenter/base/states.h"

namespace util {
namespace segmenter {
class StartProbMatrix {
 public:
  StartProbMatrix();
  ~StartProbMatrix() {}

 public:
  bool LoadStartProb(const std::string& file);
  float GetStartProb(State state) const;

 private:
  bool ParserStartProb(
      const std::string& line,
      State* state,
      float* freq);

 private:
  const static char kDefaultSplitSymbol = ':';
  const static int32 kDefaultRowNum = 2;
  const static int32 kStateNum = 4;
  const static int32 STATE = 0;
  const static int32 PROB = 1;

 private:
  float start_prob_[kStateNum];

 private:
  DISALLOW_COPY_AND_ASSIGN(StartProbMatrix);
};
}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_PUBLIC_START_PROB_MATRIX_H_
