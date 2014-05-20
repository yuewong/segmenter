// Copyright 2013 Makepolo Inc. All Rights Reserved.
// Author: huangyue@makepolo.com (Yue Huang)

#include "util/segmenter/internal/razor_wind.h"

#include <limits>

#include "base/flags.h"
#include "base/logging.h"
#include "base/likely.h"
#include "base/utf_string_conversion_utils.h"
#include "util/release/public/release.h"
#include "util/segmenter/base/states.h"
#include "util/segmenter/base/spliter.h"
#include "util/segmenter/base/utils.h"
#include "util/segmenter/public/dag_creator.h"
#include "util/segmenter/public/freq_dict.h"
#include "util/segmenter/public/punct_filter.h"
#include "util/segmenter/public/hidden_markov_model.h"

DEFINE_string(
    makepolo_segmenter_words_freq_dict_path,
    "segmenter/data/word_freq_dict.gbk.txt",
    "");

DEFINE_string(
    makepolo_segmenter_words_freq_utf8_dict_path,
    "segmenter/data/word_freq_dict.txt",
    "");

DEFINE_string(
    makepolo_segmenter_emit_prob_dict_path,
    "segmenter/data/prob_emit.gbk.txt",
    "");

DEFINE_string(
    makepolo_segmenter_emit_prob_utf8_dict_path,
    "segmenter/data/prob_emit.txt",
    "");

DEFINE_string(
    makepolo_segmenter_start_prob_dict_path,
    "segmenter/data/prob_start.txt",
    "");

DEFINE_string(
    makepolo_segmenter_trans_prob_dict_path,
    "segmenter/data/prob_trans.txt",
    "");

DEFINE_bool(
    makepolo_segmenter_filte_punct,
    true,
    "");

DEFINE_string(
    makepolo_segmenter_punct_dict_path,
    "segmenter/data/punct.gbk.txt",
    "");

DEFINE_string(
    makepolo_segmenter_punct_utf8_dict_path,
    "segmenter/data/punct.txt",
    "");

DEFINE_string(
    makepolo_segmenter_gbk_spliter_name,
    "SpliterForGBK",
    "");

DEFINE_string(
    makepolo_segmenter_utf8_spliter_name,
    "SpliterForUtf8",
    "");

namespace util {
namespace segmenter {

typedef struct {
  float freq;
  int end;
} Route;

class RazorWind::Impl {
 public:
  Impl(const SegmenterParamter* param);
  ~Impl();

 public:
  void SentenceSeg(
      const std::string& sentence,
      std::vector<std::string> *words,
      std::vector<SegmenterToken>* tokens) const;

  // for debug
  void ToStringForSimpleChar(
      const std::vector<std::string>& simple_char,
      std::string* info) const;

  void Finalize(
      const std::string& sentence,
      std::vector<std::string>* words,
      std::vector<SegmenterToken>* tokens) const;

 private:
  void CalcBestPath(
      const std::string& sentence,
      const std::vector<std::pair<int, int> >& simple_char_pos,
      Dag* dag,
      std::vector<Route>* routes) const;

  void SimpleCharMerge(
      const std::vector<std::pair<int, int> >& simple_char_pos,
      const std::string& sentence,
      std::vector<Route>* routes) const;

  void MergeHMMNewWord(
      const std::pair<int, int>& range,
      const std::vector<int>& final_path,
      std::vector<Route>* routes) const;

  void ReduceWords(
      const std::string& sentence,
      const std::vector<Route>& routes,
      const std::vector<std::pair<int, int> >& simple_char_pos,
      std::vector<std::string>* words,
      std::vector<SegmenterToken>* tokens) const;

  void FetchSimpleChineseCharForHMM(
      const std::string& sentence,
      const std::vector<Route>& routes,
      const std::vector<std::pair<int, int> >& simple_char_pos,
      std::vector<std::pair<int, int> >* simple_chinese_char_range) const;

 private:
  scoped_ptr<HiddenMarkovModel> model_;
  scoped_ptr<PunctFilter> funct_filter;
  Spliter* spliter_;
  const SegmenterParamter* paramter_;

  DagCreator dag_creator_;
  FreqDict word_freq_dict_;
};

void RazorWind::Impl::Finalize(
    const std::string& sentence,
    std::vector<std::string>* words,
    std::vector<SegmenterToken>* tokens) const {
  std::vector<std::string> new_words;
  std::vector<SegmenterToken> new_tokens;

  if (UNLIKELY(!words && !tokens)) {
    return;
  }

  if (FLAGS_makepolo_segmenter_filte_punct && funct_filter.get()) {
    const char* start = NULL;
    int32 length = 0;
    size_t size = tokens ? tokens->size() : words->size();
    for (size_t i = 0; i < size; ++i) {
      if (tokens) {
        start = &sentence[(*tokens)[i].pos_];
        length = (*tokens)[i].length_;
      } else {
        start = words->at(i).c_str();
        length = words->at(i).length();
      }
      if (!funct_filter->Filtering(start, length)) {
        if (words) {
          new_words.push_back((*words)[i]);
        }
        if (tokens) {
          new_tokens.push_back((*tokens)[i]);
        }
      }
    }
  }

  if (words) {
    words->swap(new_words);
  }

  if (tokens) {
    tokens->swap(new_tokens);
  }
}

RazorWind::Impl::Impl(const SegmenterParamter* param) :
    spliter_(NULL),
    paramter_(param),
    dag_creator_(paramter_) {
    std::string word_freq_dict_path;
    CHECK(paramter_->enc_ != SupportEncoding::INVALID);
    if (paramter_->enc_ == SupportEncoding::GBK) {
      word_freq_dict_path = release::GetRelease()->GetFullReleasePath(
      FLAGS_makepolo_segmenter_words_freq_dict_path);
    } else {
      word_freq_dict_path = release::GetRelease()->GetFullReleasePath(
      FLAGS_makepolo_segmenter_words_freq_utf8_dict_path);
    }
  CHECK(word_freq_dict_.LoadDictFile(word_freq_dict_path))
      << "load freq_dict failed";
  CHECK(dag_creator_.LoadDict()) << "load trie dict failed";

  std::string start_prob_dict_path =
    release::GetRelease()->GetFullReleasePath(
    FLAGS_makepolo_segmenter_start_prob_dict_path);
  std::string trans_prob_dict_path =
    release::GetRelease()->GetFullReleasePath(
    FLAGS_makepolo_segmenter_trans_prob_dict_path);

  std::string emit_prob_dict_path;
  if (paramter_->enc_ == SupportEncoding::GBK) {
    emit_prob_dict_path = release::GetRelease()->GetFullReleasePath(
    FLAGS_makepolo_segmenter_emit_prob_dict_path);
  } else {
    emit_prob_dict_path = release::GetRelease()->GetFullReleasePath(
    FLAGS_makepolo_segmenter_emit_prob_utf8_dict_path);
  }

  model_.reset(new HiddenMarkovModel());
  CHECK(model_.get());
  CHECK(model_->Init(
        start_prob_dict_path,
        trans_prob_dict_path,
        emit_prob_dict_path));
  if (FLAGS_makepolo_segmenter_filte_punct) {
    funct_filter.reset(new PunctFilter);
    CHECK(funct_filter.get());

    std::string punct_dict_path;
    if (paramter_->enc_ == SupportEncoding::GBK) {
      punct_dict_path = release::GetRelease()->GetFullReleasePath(
      FLAGS_makepolo_segmenter_punct_dict_path);
    } else {
      punct_dict_path = release::GetRelease()->GetFullReleasePath(
      FLAGS_makepolo_segmenter_punct_utf8_dict_path);
    }

    if (!funct_filter->Load(punct_dict_path)) {
      funct_filter.reset(NULL);
    }
  }

  if (paramter_->enc_ == SupportEncoding::GBK) {
    spliter_ = SpliterRegisterant::GetSingletonInstanceByName(
      FLAGS_makepolo_segmenter_gbk_spliter_name);
  } else {
    spliter_ = SpliterRegisterant::GetSingletonInstanceByName(
      FLAGS_makepolo_segmenter_utf8_spliter_name);
  }

  CHECK(spliter_) << "can not get spliter";
}

void RazorWind::Impl::SimpleCharMerge(
    const std::vector<std::pair<int, int> >& simple_char_pos,
    const std::string& sentence,
    std::vector<Route>* routes) const {
  if (routes->empty()) {
    return;
  }

  int32 start_pos = 0;
  int32 end_pos = -1;
  int32 state = 0;
  size_t size = routes->size();

  const char* data = sentence.c_str();
  while (state < simple_char_pos.size()) {
    if ((*routes)[state].end == state &&
        IsLetterOrDigit(&data[simple_char_pos[state].first], simple_char_pos[state].second - simple_char_pos[state].first)) {
      end_pos = state;
    } else {
      if (end_pos != -1 && end_pos - start_pos > 0) {
        (*routes)[start_pos].end = end_pos;
        end_pos = -1;
      }

      start_pos = (*routes)[state].end + 1;
    }
    state = (*routes)[state].end + 1;
  }

  if (end_pos != -1 && end_pos - start_pos > 0) {
    (*routes)[start_pos].end = end_pos;
  }
}

void RazorWind::Impl::FetchSimpleChineseCharForHMM(
    const std::string& sentence,
    const std::vector<Route>& routes,
    const std::vector<std::pair<int, int> >& simple_char_pos,
    std::vector<std::pair<int, int> >* simple_chinese_char_range) const {
  if (routes.empty()) {
    return;
  }

  int32 start_pos = 0;
  int32 end_pos = -1;
  int32 state = 0;
  size_t size = routes.size();

  const char* data = sentence.c_str();
  while (state < simple_char_pos.size()) {
    const char* start = &data[simple_char_pos[state].first];
    int32 length = simple_char_pos[state].second - simple_char_pos[state].first;
    if (routes[state].end == state &&
        !IsLetterOrDigit(start, length) &&
        !spliter_->IsPunct(start, length) &&
        length != 1) {
      end_pos = state;
    } else {
      if (end_pos != -1 && end_pos - start_pos > 0) {
        simple_chinese_char_range->push_back(std::pair<int, int>());
        simple_chinese_char_range->back().first = start_pos;
        simple_chinese_char_range->back().second = end_pos;
        end_pos = -1;
      }
      start_pos = routes[state].end + 1;
    }
    state = routes[state].end + 1;
  }

  if (end_pos != -1 && end_pos - start_pos > 0) {
    simple_chinese_char_range->push_back(std::pair<int, int>());
    simple_chinese_char_range->back().first = start_pos;
    simple_chinese_char_range->back().second = end_pos;
  }
}

void RazorWind::Impl::SentenceSeg(
    const std::string& sentence,
    std::vector<std::string>* words,
    std::vector<SegmenterToken>* tokens) const {
  std::vector<std::pair<int, int> > simple_char_pos;
  simple_char_pos.reserve(32);

  std::vector<int32> simple_char_code;
  simple_char_code.reserve(32);

  spliter_->SpiltSimpleChar(
      sentence,
      &simple_char_pos,
      &simple_char_code);


  Dag dag;
  std::vector<Route> routes;
  routes.reserve(32);

  dag_creator_.Feed(sentence, NULL, &dag, spliter_);

  CalcBestPath(sentence, simple_char_pos, &dag, &routes);

  std::vector<std::pair<int, int> > simple_chinese_range;
  simple_chinese_range.reserve(32);

  SimpleCharMerge(simple_char_pos, sentence, &routes);

  FetchSimpleChineseCharForHMM(
      sentence,
      routes,
      simple_char_pos,
      &simple_chinese_range);

  std::vector<int> final_path;
  final_path.reserve(32);
  for (int i = 0; i < simple_chinese_range.size(); ++i) {
    final_path.clear();
    model_->Viterbi(
        simple_char_code,
        simple_chinese_range[i],
        &final_path);
    MergeHMMNewWord(simple_chinese_range[i], final_path, &routes);
  }

  ReduceWords(sentence, routes, simple_char_pos, words, tokens);
}

void RazorWind::Impl::MergeHMMNewWord(
    const std::pair<int, int>& range,
    const std::vector<int>& final_path,
    std::vector<Route>* routes) const {
  if (final_path.empty()) {
    return;
  }

  int32 start_pos = -1;
  int32 end_pos = start_pos;
  for (int i = range.first; i <= range.second; ++i) {
    if (final_path[i - range.first] == util::segmenter::B) {
      start_pos = i;
    } else if (final_path[i - range.first] == util::segmenter::S) {
      start_pos = -1;
    } else if (final_path[i - range.first] == util::segmenter::E) {
      end_pos = i;
      if (start_pos != -1) {
        (*routes)[start_pos].end = end_pos;
      }
      start_pos = -1;
    }
  }
}

void RazorWind::Impl::ReduceWords(
    const std::string& sentence,
    const std::vector<Route>& routes,
    const std::vector<std::pair<int, int> >& simple_char_pos,
    std::vector<std::string>* words,
    std::vector<SegmenterToken>* tokens) const {
  if (words) {
    words->clear();
    int state = 0;
    int end_state;

    while (state < simple_char_pos.size()) {
      end_state = routes[state].end;
      if ((simple_char_pos[end_state].second - simple_char_pos[state].first == 1) &&
          sentence[simple_char_pos[state].first] == ' ') {
      } else {
        words->push_back(std::string());
        words->back().assign(
            sentence.data(),
            simple_char_pos[state].first,
            simple_char_pos[end_state].second - simple_char_pos[state].first);
      }
      state = end_state + 1;
    }
  }

  if (tokens) {
    tokens->clear();
    int state = 0;
    int end_state;

    while (state < simple_char_pos.size()) {
      end_state = routes[state].end;
      if ((simple_char_pos[end_state].second - simple_char_pos[state].first == 1) &&
          sentence[simple_char_pos[state].first] == ' ') {
      } else {
        tokens->push_back(SegmenterToken(
              simple_char_pos[state].first,
              simple_char_pos[end_state].second - simple_char_pos[state].first));
      }
      state = end_state + 1;
    }
  }
}

void RazorWind::Impl::CalcBestPath(
    const std::string& sentence,
    const std::vector<std::pair<int, int> >& simple_char_pos,
    Dag* dag,
    std::vector<Route>* routes) const {
  float freq = 0.0f;

  int state_num = dag->StateCount();

  routes->clear();
  routes->resize(state_num + 1);
  (*routes)[state_num].freq = 0.0f;

  for (int i = state_num - 1; i >= 0; --i) {
    int end_state_num = (*dag)[i].size();

    float max_freq = -std::numeric_limits<float>::max();
    int best_stat;
    const char* data = sentence.c_str();
    for (int j = 0; j < end_state_num; ++j) {
      float freq;
      if (!word_freq_dict_.Find(
            data + simple_char_pos[i].first,
            simple_char_pos[(*dag)[i][j]].second - simple_char_pos[i].first,
            &freq)) {
        freq = word_freq_dict_.GetMinFreq();
      }

      if (freq + (*routes)[(*dag)[i][j] + 1].freq > max_freq) {
        max_freq = freq + (*routes)[(*dag)[i][j] + 1].freq;
        best_stat = (*dag)[i][j];
      }
    }

    (*routes)[i].freq = max_freq;
    (*routes)[i].end = best_stat;
  }
}

void RazorWind::Impl::ToStringForSimpleChar(
    const std::vector<std::string>& simple_char,
    std::string* info) const {
  size_t size = simple_char.size();
  std::string str;
  for (size_t i = 0; i < size; ++i) {
    SStringPrintf(&str, "[ %d : ", i);
    str.append(simple_char[i]);
    str.append(" ] ");
    info->append(str);
  }
}


RazorWind::Impl::~Impl() {
}

RazorWind::RazorWind(const SegmenterParamter& param) : Segmenter(param) {
  impl_.reset(new RazorWind::Impl(&paramter_));
  CHECK(impl_.get());
}

RazorWind::~RazorWind() {
}

void RazorWind::FeedInternal(
    const std::string& sentence,
    std::vector<std::string>* words,
    std::vector<SegmenterToken>* tokens) const {
  impl_->SentenceSeg(sentence, words, tokens);
}

void RazorWind::Finalize(
    const std::string& sentence,
    std::vector<std::string>* words,
    std::vector<SegmenterToken>* tokens) const {
  impl_->Finalize(sentence, words, tokens);
}

}  // segmenter
}  // util
