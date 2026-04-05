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

#include <turbo/utility/status.h>

namespace gnef::api {
    class DictManager {
    public:
        static DictManager &instance() {
            static DictManager dict;
            return dict;
        }

        turbo::Result<std::vector<std::pair<std::string, size_t> > > dump_default_dict(
            bool reset = false, std::string root = "");

        std::string hadar_dict() const;

        std::string xpinyin_dict() const;

        std::string fasttext_dict() const;

        std::string jieba_dict() const;

        std::string ngram_dict() const;
        std::string intent_dict() const;
        std::string rewrite_dict() const;
        std::string ner_dict() const;

        const std::string &root() const {
            return _root;
        }

        bool reseted() const {
            return _reset;
        }

        std::string default_hadar_dict() const;

        std::string default_xpinyin_dict() const;

        std::string default_fasttext_dict() const;

        std::string default_jieba_dict() const;

        std::string default_ner_dict() const;
        std::string default_intent_dict() const;
        std::string default_rewrite_dict() const;
        std::string default_ngram_dict() const;

        const turbo::Result<std::vector<std::pair<std::string, size_t> > > &result() const {
            return _result;
        }

    private:
        turbo::Result<std::vector<std::pair<std::string, size_t> > > dump_default_dict_internal(
            bool reset, std::string root);

        turbo::Status dump_default_jieba_dict(bool reset,
                                              std::vector<std::pair<std::string, size_t> > &info);

        turbo::Status dump_default_fasttext_dict(bool reset,
                                                 std::vector<std::pair<std::string, size_t> > &info);

        turbo::Status dump_default_xpinyin_dict(bool reset,
                                                std::vector<std::pair<std::string, size_t> > &info);

        turbo::Status dump_default_hadar_dict(bool reset,
                                              std::vector<std::pair<std::string, size_t> > &info);

    private:
        turbo::Result<std::vector<std::pair<std::string, size_t> > > _result;
        std::string _root;
        bool _reset{false};
        std::string _hadar_dict;
        std::string _xpinyin_dict;
        std::string _fasttext_dict;
        std::string _jieba_dict;
        std::string _ner_dict;
        std::string _intent_dict;
        std::string _rewrite_dict;
        std::string _ngram_dict;
    };
} // namespace gnef::api
