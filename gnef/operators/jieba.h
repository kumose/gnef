// Copyright (C) 2026 Kumo inc. and its affiliates. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

#include  <jieba/jieba.h>
#include <turbo/utility/status.h>
#include <gnef/instance/double_instance.h>
#include <gnef/api/segment.h>
namespace gnef::api {


    class JiebaHandler : public Segmenter {
    public:

        JiebaHandler() = default;
        ~JiebaHandler() = default;



        turbo::Status cut(const std::string &sentence, std::vector<std::string> &words, bool hmm = true) const {
            return _jieba.cut(sentence, words, hmm);
        }

        turbo::Status cut(const std::string &sentence, std::vector<kmjieba::Word> &words, bool hmm = true) const {
            return _jieba.cut(sentence, words, hmm);
        }

        turbo::Status cut_all(const std::string &sentence, std::vector<std::string> &words) const {
            return _jieba.cut(sentence, words);
        }

        turbo::Status cut_all(const std::string &sentence, std::vector<kmjieba::Word> &words) const {
            return _jieba.cut(sentence, words);
        }

        turbo::Status
        cut_for_search(const std::string &sentence, std::vector<std::string> &words, bool hmm = true) const {
            return _jieba.cut(sentence, words, hmm);
        }

        turbo::Status cut_for_search(const std::string &sentence, std::vector<kmjieba::Word> &words, bool hmm = true) const {
            return _jieba.cut(sentence, words, hmm);
        }

        turbo::Status cut_hmm(const std::string &sentence, std::vector<std::string> &words) const {
            return _jieba.cut(sentence, words);
        }

        turbo::Status cut_hmm(const std::string &sentence, std::vector<kmjieba::Word> &words) const {
            return _jieba.cut(sentence, words);
        }

        turbo::Status
        cut_small(const std::string &sentence, std::vector<std::string> &words, size_t max_word_len) const {
            return _jieba.cut(sentence, words, max_word_len);
        }

        turbo::Status cut_small(const std::string &sentence, std::vector<kmjieba::Word> &words, size_t max_word_len) const {
            return _jieba.cut(sentence, words, max_word_len);
        }

        void tag(const std::string &sentence, std::vector<std::pair<std::string, std::string> > &words) const {
            _jieba.tag(sentence, words);
        }

        std::string lookup_tag(const std::string &str) const {
            return _jieba.lookup_tag(str);
        }

        turbo::Status insert_user_word(const std::string &word, const std::string &tag = kmjieba::UNKNOWN_TAG) {
            return _jieba.insert_user_word(word, tag);
        }

        turbo::Status insert_user_word(const std::string &word, int freq, const std::string &tag = kmjieba::UNKNOWN_TAG) {
            return _jieba.insert_user_word(word, freq, tag);
        }

        turbo::Status delete_user_word(const std::string &word, const std::string &tag = kmjieba::UNKNOWN_TAG) {
            return _jieba.delete_user_word(word, tag);
        }

        bool find(const std::string &word) {
            return _jieba.find(word);
        }

        ////////////////////////////////////////////
        ///  dict names, this function is for reload
        ///  jieba instance for user dict, default,
        ///  we use system default dicts, see
        ///  initialize_default.
        ///  NOTE, if you using your own dict for system
        ///  default, reset the GnefConfig::set_dict_config,
        ///  set you config before initialize_gnef,so jieba_dict()
        ///  will using you directory dicts,but guard that, make the
        ///  the dict names as below, add a defaylt_ for prefix
        /// dict_path -> jieba.dict.utf8
        /// user_dict_path -> user.dict.utf8
        /// model_path -> hmm_model.utf8
        /// idf_path -> idf.utf8
        /// stop_word_path -> stop_words.utf8
        turbo::Status initialize(const std::string & dict_dir);

        turbo::Status segment(const kumo::nlp::SegmentRequest &req, kumo::nlp::SegmentResponse &res) const override;
    private:
        friend class SegmentorInstance;

        ////////////////////////////////////////////
        ///  dict names, this dict is system default
        ///  dump by gnef/api/dict.h:dump_default_jieba_dict
        /// dict_path -> default_jieba.dict.utf8
        /// user_dict_path -> default_user.dict.utf8
        /// model_path -> default_hmm_model.utf8
        /// idf_path -> default_idf.utf8
        /// stop_word_path -> default_stop_words.utf8
        turbo::Status initialize_default(const std::string & dict_dir);

    private:
        std::string _dict_path;
        kmjieba::Jieba _jieba;
    };
} // namespace gnef::api
