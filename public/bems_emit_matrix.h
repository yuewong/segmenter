// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_BEMS_EMIT_MATRIX_H_
#define UTIL_SEGMENTER_PUBLIC_BEMS_EMIT_MATRIX_H_

#include "base/scoped_ptr.h"
#include "util/segmenter/base/states.h"
#include "util/sparsehash/public/dense_hash_map.h"

namespace util {
namespace segmenter {
class BemsEmitMatrix {
 public:
  BemsEmitMatrix();
  ~BemsEmitMatrix();

 public:
  bool LoadModel(const std::string& file_path);
  bool GetEmitProb(State state, int32 code, float *prob) const;
  bool GetEmitProb(State state, const std::string& str, float *prob) const;

 private:
  bool ParserStateEmit(
      const std::string& line,
      State* state,
      int32* code,
      float* freq);

 private:
  const static int32 kStateNum = 4;
  const static int32 kDefaultEmitMatrixSize = 16383;
  const static char  kDefaultSplitSymbol = ':';
  const static int32 kDefaultMatrixNum = 3;
  const static int32 STATE_NAME = 0;
  const static int32 CHINESE_CHAR = 1;
  const static int32 PROB = 2;

  typedef ::util::google::dense_hash_map<int32, float> EmitMatrix;

  bool is_loaded_;
  int32 emit_num_;
  EmitMatrix* emit_matrix_[kStateNum];

 private:
  DISALLOW_COPY_AND_ASSIGN(BemsEmitMatrix);
};
}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_PUBLIC_BEMS_EMIT_MATRIX_H_
