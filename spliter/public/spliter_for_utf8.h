// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_SPLITER_PUBLIC_SPLITER_FOR_UTF8_H_
#define UTIL_SEGMENTER_SPLITER_PUBLIC_SPLITER_FOR_UTF8_H_

#include "base/basictypes.h"
#include "util/segmenter/base/spliter.h"

namespace util {
namespace segmenter {

class SpliterForUtf8 : public Spliter {
 public:
  SpliterForUtf8() {}
  virtual ~SpliterForUtf8() {}

 public:
  virtual void SpiltSimpleChar(
      const std::string& sentence,
      // std::vector<std::string>* simple_char,
      std::vector<std::pair<int, int> >* simple_char_pos,
      std::vector<int>* simple_char_code);

  virtual bool GetNextChar(
      const char* data,
      int start_pos,
      int length,
      int* end_pos,
      int* code);

  virtual bool IsPunct(const std::string& str);
  virtual bool IsPunct(const char* data, int32 length);

 private:
  DISALLOW_COPY_AND_ASSIGN(SpliterForUtf8);
};

}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_SPLITER_PUBLIC_SPLITER_FOR_UTF8_H_
