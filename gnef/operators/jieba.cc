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

#include <mutex>
#include <gnef/operators/jieba.h>
#include <turbo/random/rand_util.h>
#include <turbo/files/filesystem.h>
#include <xpinyin/dump_dict.h>
#include <xpinyin/g2p_global.h>

namespace gnef::api {


    turbo::Status JiebaHandler::initialize_default(const std::string &dict_dir) {
        _dict_path = dict_dir;
        auto dict_path = (turbo::FilePath(_dict_path)/"default_jieba.dict.utf8").string();
        auto user_dict_path = (turbo::FilePath(_dict_path)/"default_user.dict.utf8").string();
        auto model_path = (turbo::FilePath(_dict_path)/"default_hmm_model.utf8").string();
        auto idf_path = (turbo::FilePath(_dict_path)/"default_idf.utf8").string();
        auto stop_word_path = (turbo::FilePath(_dict_path)/"default_stop_words.utf8").string();

        TURBO_RETURN_NOT_OK(_jieba.initialize(dict_path ,
                                 model_path ,
                                 user_dict_path,
                                 idf_path,
                                 stop_word_path));
        return turbo::OkStatus();
    }

    ////////////////////////////////////////////
    ///  dict names
    /// dict_path -> jieba.dict.utf8
    /// user_dict_path -> user.dict.utf8
    /// model_path -> hmm_model.utf8
    /// idf_path -> idf.utf8
    /// stop_word_path -> stop_words.utf8
    turbo::Status JiebaHandler::initialize(const std::string &dict_dir) {
        _dict_path = dict_dir;
        auto dict_path = (turbo::FilePath(_dict_path)/"jieba.dict.utf8").string();
        auto user_dict_path = (turbo::FilePath(_dict_path)/"user.dict.utf8").string();
        auto model_path = (turbo::FilePath(_dict_path)/"hmm_model.utf8").string();
        auto idf_path = (turbo::FilePath(_dict_path)/"idf.utf8").string();
        auto stop_word_path = (turbo::FilePath(_dict_path)/"stop_words.utf8").string();

        TURBO_RETURN_NOT_OK(_jieba.initialize(dict_path ,
                                 model_path ,
                                 user_dict_path,
                                 idf_path,
                                 stop_word_path));
        return turbo::OkStatus();
    }


    static void merge_jieba_to_pb(const kmjieba::ExtractorWord &word,kumo::nlp::SegmentTerm &out) {
        auto oterm = out.mutable_main_term();
        oterm->set_term(word.word);
        oterm->set_idf(word.weight);
        auto *ooffset = oterm->mutable_offset()->Add()->mutable_offsets();
        ooffset->Reserve(word.offsets.size());
        for (auto &offset : word.offsets) {
            auto *os = ooffset->Add();
            os->set_offset(offset.offset);
            os->set_unicode_length(offset.unicode_length);
            os->set_unicode_offset(offset.unicode_offset);
        }

    }

    turbo::Status JiebaHandler::segment(const kumo::nlp::SegmentRequest &req, kumo::nlp::SegmentResult &res) const {
        static constexpr int kTermLimited = 10;
        auto &q = req.query();
        auto enable_pos = req.setting().enable_pos();
        int top_n = req.setting().limited();
        if (top_n <= 0) {
            top_n = kTermLimited;
        }
        std::vector<kmjieba::ExtractorWord> words;
        auto *mutable_list = res.mutable_terms();
        TURBO_RETURN_NOT_OK(_jieba.extractor.extract(q, words, top_n));

        /// avoid if branch in for loop
        mutable_list->Reserve(words.size());
        std::cerr << words.size() << " words " <<words<< std::endl;
        if (enable_pos) {
            for (auto &w : words) {
                kumo::nlp::SegmentTerm term;
                merge_jieba_to_pb(w, term);
                auto pos = _jieba.lookup_tag(w.word);
                term.mutable_main_term()->set_pos(pos);
                *mutable_list->Add() = std::move(term);
            }
        } else {
            for (auto &w : words) {
                kumo::nlp::SegmentTerm term;
                merge_jieba_to_pb(w, term);
                *mutable_list->Add() = std::move(term);
            }
        }
        return turbo::OkStatus();
    }
}  // namespace gnef::api

