// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#include "util/segmenter/internal/dag_creator.h"

#include <string>

#include "base/flags.h"
#include "base/logging.h"
#include "base/icu_utf.h"
#include "base/likely.h"
#include "base/utf_string_conversion_utils.h"
#include "util/double_array_trie/internal/double_array_trie.h"
#include "util/file/public/simple_file_reader.h"
#include "util/release/public/release.h"

DEFINE_string(
    segmenter_dict_path,
    "segmenter/data/base_words.gbk.txt",
    "default base_words path");

DEFINE_string(
    segmenter_dict_utf8_path,
    "segmenter/data/base_words.txt",
    "default base_words path");

namespace util {
namespace segmenter {

class DagCreator::Impl {
 public:
  Impl(const SegmenterParamter* param) : param_(param) {
    trie_.reset(new util::DoubleArrayTrie());
    CHECK(trie_.get()) << "trie generation failed";
  }
  ~Impl() {}

 public:
  bool LoadDict() {
    if (FLAGS_segmenter_dict_path.empty()) {
      CHECK(false) << "not set dictionary";
    }

    std::string dict_path;
    if (param_->enc_ == SupportEncoding::GBK) {
      dict_path = release::GetRelease()->GetFullReleasePath(
        FLAGS_segmenter_dict_path);
    } else {
      dict_path = release::GetRelease()->GetFullReleasePath(
        FLAGS_segmenter_dict_utf8_path);
    }

    util::file::SimpleFileReader reader(dict_path);
    std::string line;
    while (reader.ReadLine(&line)) {
      trie_->AddString(line);
    }

    if (trie_->GetRecordCount() == 0) {
      CHECK(false) << "load base_words dict failed";
    }

    return BuildTrie();
  }

  bool BuildTrie() {
    return trie_->Build();
  }

  void Feed(
      const std::string& sentent,
      std::vector<std::string>* words,
      Dag* dag,
      Spliter* spliter) const {
    const char* data = sentent.c_str();
    size_t length = sentent.length();
    GenerateAllWords(data, length, words, dag, spliter);
  }

 private:
  void GenerateAllWords(
      const char* data,
      size_t length,
      std::vector<std::string>* words,
      Dag* dag,
      Spliter* spliter) const {
    int32 code_point = 97;
    int32 word_start = 0;
    int32 word_end = 0;
    int32 next_char = 0;
    int32 char_num = 0;
    int32 start_state = 0;
    int32 next_state = 0;
    bool is_sub = false;
    bool find;

    while (true) {
      if (!spliter->GetNextChar(
            data,
            word_start,
            length,
            &next_char,
            &code_point)) {
        return;
      }

      word_end = next_char;
      next_state = start_state;
      is_sub = false;
      while ((find = trie_->Search(data, word_start, word_end, &is_sub)) ||
             is_sub) {
        if (find) {
          if (words) {
            words->push_back(std::string());
            words->back().assign(data, word_start, word_end - word_start);
          }
          dag->AddNewState(start_state);
          dag->AddStateMap(start_state, next_state);
        }

        if (!spliter->GetNextChar(
              data,
              word_end,
              length,
              &word_end,
              &code_point)) {
            char_num++;
            next_state++;
            break;
        }
        char_num++;
        next_state++;
      }

      if (!trie_->Search(data, word_start, next_char)) {
      // simple char
        // add words
        if (words) {
          words->push_back(std::string());
          words->back().assign(data, word_start, word_end - word_start);
        }
        dag->AddNewState(start_state);
        dag->AddStateMap(start_state, start_state);
      }
      word_start = next_char;
      start_state++;
      char_num = 0;
    }
  }

 private:
  scoped_ptr<util::DoubleArrayTrie> trie_;
  const SegmenterParamter* param_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Impl);
};

DagCreator::DagCreator(const SegmenterParamter* param) {
  impl_.reset(new DagCreator::Impl(param));
  CHECK(impl_.get());
}

void DagCreator::Feed(
    const std::string& sentent,
    std::vector<std::string>* words,
    Dag* dag,
    Spliter* spliter) const {
  impl_->Feed(sentent, words, dag, spliter);
}

DagCreator::~DagCreator() {
}

bool DagCreator::LoadDict() {
  return impl_->LoadDict();
}

bool DagCreator::BuildTrie() {
  return impl_->BuildTrie();;
}
}  // namespace file
}  // namespace util
