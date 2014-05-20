// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: huangyue@jike.com (Yue Huang)

#ifndef UTIL_SEGMENTER_PUBLIC_FREQ_DICT_H_
#define UTIL_SEGMENTER_PUBLIC_FREQ_DICT_H_

#include "base/hash_tables.h"
#include "util/dict/public/dict_base.h"
#include "util/sparsehash/public/dense_hash_map.h"

namespace util {
namespace segmenter {

class FreqDict : public util::dict::PayloadDict<std::string, float> {
 public:
  FreqDict();
  virtual ~FreqDict() {}

 public:
  bool Find(const std::string& elem, float* value) const;

  bool Find(const char* src, int32 length, float* value) const;

  int32 GetWordCount() const {
    return word_num_;
  }

  float GetMinFreq() const {
    return min_freq_;
  }

 protected:
  bool Finalize();

  bool Parse(const std::string& line, std::string* key, float* value);

  bool ProcessLineInternal(const std::string& key, const float& value);

 private:
  util::google::dense_hash_map<uint64, float> freq_dict_;
  int32 word_num_;
  float min_freq_;
};

}  // namespace segmenter
}  // namespace util
#endif  // UTIL_SEGMENTER_PUBLIC_FREQ_DICT_H_
