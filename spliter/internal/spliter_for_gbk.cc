// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#include "util/segmenter/spliter/internal/spliter_for_gbk.h"

#include "base/utf_string_conversion_utils.h"

namespace util {
namespace segmenter {
void SpliterForGBK::SpiltSimpleChar(
    const std::string& sentence,
    std::vector<std::pair<int, int> >* simple_char_pos,
    std::vector<int>* simple_char_code) {
  simple_char_pos->clear();
  simple_char_code->clear();

  const char* data = sentence.c_str();
  int32 start_pos = 0;
  int32 length = sentence.size();
  int32 code;

  while (start_pos < length) {
    if (data[start_pos] & (char)0x80) {
      if (start_pos + 1 < length) {
       simple_char_pos->push_back(std::pair<int, int>());
       simple_char_pos->back().first = start_pos;
       simple_char_pos->back().second = start_pos + 2;

        // > 127
        const unsigned char* high =
          reinterpret_cast<const unsigned char*>(&data[start_pos]);
        const unsigned char* low =
          reinterpret_cast<const unsigned char*>(&data[start_pos + 1]);
        simple_char_code->push_back(GetGbkCode(*high, *low));
      }
      start_pos += 2;
      continue;
    } else {
      simple_char_pos->push_back(std::pair<int, int>());
      simple_char_pos->back().first = start_pos;
      simple_char_pos->back().second = start_pos + 1;

      simple_char_code->push_back(data[start_pos]);
      ++start_pos;
    }
  }
}

bool SpliterForGBK::GetNextChar(
    const char* data,
    int start_pos,
    int length,
    int* end_pos,
    int* code) {
  if (start_pos >= length) {
    return false;
  }

  if ((data[start_pos] & (char)0x80)) {
    if (start_pos + 1 < length) {
      const unsigned char* high =
        reinterpret_cast<const unsigned char*>(&data[start_pos]);
      const unsigned char* low =
        reinterpret_cast<const unsigned char*>(&data[start_pos + 1]);
      *code = GetGbkCode(*high, *low);
      *end_pos = start_pos + 2;
      return true;
    } else {
      return false;
    }
  }

  *code = data[start_pos];
  *end_pos = start_pos + 1;
  return true;
}

bool SpliterForGBK::IsPunct(const std::string& str) {
  return IsPunct(str.c_str(), str.length());
}

bool SpliterForGBK::IsPunct(const char* data, int32 length) {
  if (length != 2) {
    return true;
  }

  const unsigned char* high =
    reinterpret_cast<const unsigned char*>(&data[0]);
  const unsigned char* low =
    reinterpret_cast<const unsigned char*>(&data[1]);

  int32 code = GetGbkCode(*high, *low);

  if (code <= 0xA9FE && code >= 0xA1A1) {
    return true;
  }

  if (code <= 0xA840 && code >= 0xA9A0) {
    return true;
  }

  return false;
}

REGISTER_CLASS(Spliter, SpliterForGBK)
}  // segmenter
}  // util
