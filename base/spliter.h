// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_BASE_SPLITER_H_
#define UTIL_SEGMENTER_BASE_SPLITER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/small_vector.h"
#include "util/registrant/public/registrant.h"

namespace util {
namespace segmenter {

class Spliter {
 public:
  Spliter() {}
  virtual ~Spliter() {}

 public:
  virtual void SpiltSimpleChar(
      const std::string& sentence,
      std::vector<std::pair<int, int> >* simple_char_pos,
      std::vector<int>* simple_char_code) = 0;

  virtual bool GetNextChar(
      const char* data,
      int start_pos,
      int length,
      int* end_pos,
      int* code) = 0;

  virtual bool IsPunct(const std::string& str) = 0;
  virtual bool IsPunct(const char* data, int32 length) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(Spliter);
};

REGISTERANT(Spliter);
}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_BASE_SPLITER_H_
