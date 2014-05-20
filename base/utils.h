// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#ifndef UTIL_SEGMENTER_BASE_UTILS_H_
#define UTIL_SEGMENTER_BASE_UTILS_H_

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/utf_string_conversion_utils.h"
#include "util/segmenter/base/spliter.h"

namespace util {
namespace segmenter {

template<class Type>
class SimpleTwoDimArray {
 public:
  explicit SimpleTwoDimArray(int row, int list) :
      row_(row), list_(list), buff_(NULL) {
    DCHECK(row > 0);
    DCHECK(list > 0);
    buff_ = new Type[row * list];
  }
  ~SimpleTwoDimArray() {
    delete[] buff_;
  }

  Type& Value(int r, int l) {
    DCHECK(r * list_ + l < row_ * list_);
    DCHECK(r >= 0);
    DCHECK(l >= 0) << " r = " << r << " l = " << l;
    return buff_[r * list_ + l];
  }

 private:
  int row_;
  int list_;
  Type* buff_;
};

inline bool IsLetterOrDigit(char ch) {
  if ((ch <= '9' && ch >= '0') ||
      (ch <= 'Z' && ch >= 'A') ||
      (ch <= 'z' && ch >= 'a') ||
      (ch == '.')) {
    return true;
  }

  return false;
}

inline bool IsLetterOrDigit(const std::string& str) {
  if (str.size() == 1 && IsLetterOrDigit(str[0])) {
    return true;
  }
  return false;
}

inline bool IsLetterOrDigit(const char* str, int length) {
  if (length == 1 && IsLetterOrDigit(str[0])) {
    return true;
  }
  return false;
}

inline bool ConvertToUtf8Code(const std::string& str, int32* code) {
  if (str.size() != 4) {
    return false;
  }

  *code = 0;
  for (int i = 0; i < 4; ++i) {
    char c = str[i];
    int index = 0;
    if ((c <= '9' && c >= '0')) {
      index = c - '0';
    } else if ((c <= 'f' && c >= 'a')) {
      index = c - 'a' + 10;
    } else {
      return false;
    }

    *code <<= 4;
    *code += index;
  }
  return true;
}

inline bool GetSimpleChineseStrUtf8Code(
    const char* str,
    int32 length,
    int32* code) {
  if (length > 3) {
    return false;
  }

  int32 end;
  return base::GetNextUTF8Char(str, 0, 3, &end, code);
}

inline bool GetSimpleChineseStrUtf8Code(const std::string& str, int32* code) {
  return GetSimpleChineseStrUtf8Code(str.c_str(), str.length(), code);
}

inline bool IsPunct(const std::string& str) {
  int32 code = 0;
  bool ret = GetSimpleChineseStrUtf8Code(str, &code);
  if (!ret) {
    return false;
  }

  // full-width ASCII characters, full-width chinese and english puncts,
  // half-width katakana, half-width hiragana, half-width of the korean alphabet
  if (code >= 0xff00 && code <= 0xffef) {
    return true;
  }

  // ! " # ... ... /
  if (code >= 33 && code <= 47) {
    return true;
  }

  //
  if (code >= 58 && code <= 64) {
    return true;
  }

  if (code >= 91 && code <= 96) {
    return true;
  }

  // chinese code
  if (code >= 0x4e00 && code <= 0x9fa5) {
    return false;
  }
  return true;
}

inline bool IsPunct(const std::string& str, Spliter* spliter) {
  return true;
}
}  // segmenter
}  // util

#endif  // UTIL_SEGMENTER_BASE_UTILS_H_
