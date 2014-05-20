// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_DAG_H_
#define UTIL_SEGMENTER_PUBLIC_DAG_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/scoped_ptr.h"
#include "base/string_util.h"
#include "util/sparsehash/public/sparse_hash_map.h"

namespace util {
namespace segmenter {

class Dag {
 public:
  Dag() {
    state_.reserve(1024);
  }
  ~Dag() {}

 public:
  std::vector<int>& operator[] (int32 index) {
    return state_[index];
  }

  void AddNewState(int index) {
    if (index < state_.size()) {
      return;
    }
    state_.push_back(std::vector<int32>());
    state_.back().reserve(4);
  }

  void AddStateMap(int32 index, int32 state) {
    state_[index].push_back(state);
  }

  size_t StateCount() {
    return state_.size();
  }

  void Clear() {
    std::vector<std::vector<int32> >::iterator iter = state_.begin();
    for (; iter != state_.end(); iter++) {
      iter->clear();
    }
  }

  // for debug
  void ToString(std::string* info) const {
    std::vector<std::vector<int32> >::const_iterator iter = state_.begin();
    int j = 0;
    for (; iter != state_.end(); iter++) {
      std::string state;
      SStringPrintf(&state, "{ %d : [ ", j);
      for(int i = 0; i < iter->size(); ++i) {
        std::string state_map;
        SStringPrintf(&state_map, "%d, ", (*iter)[i]);
        state.append(state_map);
      }
      state.append(" ] }");
      info->append(state);
      info->append(" ; ");
      ++j;
    }
  }

 private:
  std::vector<std::vector<int32> > state_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Dag);
};

}  // namespace file
}  // namespace util
#endif  // UTIL_SEGMENTER_PUBLIC_DAG_H_
