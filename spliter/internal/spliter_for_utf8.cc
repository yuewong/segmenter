// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#include "util/segmenter/spliter/internal/spliter_for_utf8.h"

#include "base/utf_string_conversion_utils.h"
#include "util/segmenter/base/utils.h"

namespace util {
namespace segmenter {
void SpliterForUtf8::SpiltSimpleChar(
    const std::string& sentence,
    // std::vector<std::string>* simple_char,
    std::vector<std::pair<int, int> >* simple_char_pos,
    std::vector<int>* simple_char_code) {
  simple_char_pos->clear();
  simple_char_code->clear();

  const char* data = sentence.c_str();
  int32 start_pos = 0;
  int32 end_pos;
  int32 length = sentence.size();
  int32 code;

  while (base::GetNextUTF8Char(data, start_pos, length, &end_pos, &code)) {
    simple_char_code->push_back(code);
    simple_char_pos->push_back(std::pair<int, int>());
    simple_char_pos->back().first = start_pos;
    simple_char_pos->back().second = end_pos;
    start_pos = end_pos;
  }
}

bool SpliterForUtf8::GetNextChar(
    const char* data,
    int start_pos,
    int length,
    int* end_pos,
    int* code) {
  return base::GetNextUTF8Char(data, start_pos, length, end_pos, code);
}

bool SpliterForUtf8::IsPunct(const std::string& str) {
  return IsPunct(str.c_str(), str.length());
}

bool SpliterForUtf8::IsPunct(const char* str, int32 length) {
  int32 code = 0;
  bool ret = GetSimpleChineseStrUtf8Code(str, length, &code);

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

  if (code >= 58 && code <= 64) {
    return true;
  }

  if (code >= 91 && code <= 96) {
    return true;
  }

  if (code >= 0x4e00 && code <= 0x9fa5) {
    return false;
  }

  return true;
}

REGISTER_CLASS(Spliter, SpliterForUtf8)
}  // segmenter
}  // util
