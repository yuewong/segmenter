// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: huangyue@jike.com (Yue Huang)

#include "util/segmenter/internal/freq_dict.h"

#include <math.h>
#include <vector>

#include "base/likely.h"
#include "base/string_util.h"
#include "index/utils/token_utils.h"

namespace util {
namespace segmenter {
FreqDict::FreqDict() : word_num_(0), min_freq_(1) {
  freq_dict_.set_empty_key(0);
}

bool FreqDict::Parse(const std::string& line, std::string* key, float* value) {
  DCHECK(key);
  DCHECK(value);
  std::vector<std::string> v;

  SplitString(line, DictBase::kColumnSeparator, &v);
  if (UNLIKELY(v.size() != (size_t)(DictBase::kDoubleColumn))) {
    return false;
  }

  double freq;
  if (!StringToDouble(v[1], &freq)) {
    return false;
  }

  *value = freq;
  *key = v[0];
  return true;
}

bool FreqDict::Finalize() {
  util::google::dense_hash_map<uint64, float>::iterator iter2 =
      freq_dict_.begin();
  for (; iter2 != freq_dict_.end(); ++iter2) {
    iter2->second = log(iter2->second / word_num_);
    if (iter2->second < min_freq_) {
      min_freq_ = iter2->second;
    }
  }
  return true;
}

bool FreqDict::Find(const std::string& elem, float* value) const {
  return Find(elem.c_str(), elem.length(), value);
}

bool FreqDict::Find(const char* src, int32 length, float* value) const {
  uint64 hash = indexing::FNV64(src, length);
  util::google::dense_hash_map<uint64, float>::const_iterator c_iter =
    freq_dict_.find(hash);
  if (c_iter != freq_dict_.end()) {
    if (value) {
      *value = c_iter->second;
    }
    return true;
  }
  return false;
}

bool FreqDict::ProcessLineInternal(const std::string& key, const float& value) {
  uint64 hash = indexing::FNV64(key.c_str(), key.size());
  freq_dict_[hash] = value;
  word_num_ += (int)value;
  return true;
}

}  // namespace segmenter
}  // namespace util
