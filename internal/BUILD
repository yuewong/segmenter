# Copyright 2013 Makepolo Inc. All Rights Reserved.
# Author: huangyue@makepolo.com (Yue Huang)

cc_library(
    name = 'dag_creator_internal',
    srcs = [
           'dag_creator.cc',
           ],
    deps = [
           '//base:base',
           '//util/double_array_trie/public:double_array_trie',
           '//util/file/public:simple_file_reader',
           '//util/release/public:release',
           '//util/multi_pattern_search/public:mutil_pattern_strategy',
           ],
)

cc_library(
    name = 'freq_dict_internal',
    srcs = [
           'freq_dict.cc',
           ],
    deps = [
           '//base:base',
           '//util/dict/public:dict_base',
           ],
)

cc_library(
    name = 'tokenid_dict_internal',
    srcs = [
           'tokenid_dict.cc',
           ],
    deps = [
           '//base:base',
           '//util/dict/public:dict_base',
           ],
)

cc_library(
    name = 'razor_wind_internal',
    srcs = [
           'razor_wind.cc',
           ],
    deps = [
           ':freq_dict_internal',
           ':dag_creator_internal',
           ':hidden_markov_model_internal',
           ':punct_filter_internal',
           '//base:base',
           '//util/release/public:release',
           '//util/segmenter/spliter/public:spliters',
           ],
)

cc_library(
    name = 'bems_emit_matrix_internal',
    srcs = [
           'bems_emit_matrix.cc',
           ],
    deps = [
           '//base:base',
           '//util/file/public:simple_file_reader',
           ],
)

cc_library(
    name = 'hidden_markov_model_internal',
    srcs = [
           'hidden_markov_model.cc',
           ],
    deps = [
           '//base:base',
           '//util/file/public:simple_file_reader',
           ':state_trans_matrix_internal',
           ':bems_emit_matrix_internal',
           ':start_prob_matrix_internal',
           ],
)

cc_library(
    name = 'state_trans_matrix_internal',
    srcs = [
           'state_trans_matrix.cc',
           ],
    deps = [
           '//base:base',
           '//util/file/public:simple_file_reader',
           ],
)

cc_library(
    name = 'start_prob_matrix_internal',
    srcs = [
           'start_prob_matrix.cc',
           ],
    deps = [
           '//base:base',
           '//util/file/public:simple_file_reader',
           ],
)

cc_library(
    name = 'punct_filter_internal',
    srcs = [
           'punct_filter.cc',
           ],
    deps = [
           ':tokenid_dict_internal',
           '//base:base',
           '//util/file/public:simple_file_reader',
           '//util/dict/public:string_dict',
           ],
)
