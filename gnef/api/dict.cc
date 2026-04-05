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

#include <gnef/api/dict.h>
#include <hadar/data/xxd_gen.h>
#include <turbo/files/filesystem.h>
#include <turbo/files/file_util.h>
#include <gnef/api/config.h>
#include <xpinyin/xxd_gen.h>
#include <gnef/xxd_gen.h>
#include <jieba/xxd_gen.h>
#include  <mutex>

namespace gnef::api {
    turbo::Result<std::pair<std::string, size_t> > dump_dict(std::string_view prefix, std::string_view dict,
                                                             std::string_view content) {
        auto path = turbo::FilePath(prefix) / dict;
        auto dir = path.parent_path();
        TURBO_UNUSED(turbo::create_directories(dir));
        TURBO_RETURN_NOT_OK(turbo::write_to_file(path, content));
        return std::pair<std::string, size_t>{std::string(path), content.size()};
    }

    std::once_flag dump_dict_once_flag;

    turbo::Result<std::vector<std::pair<std::string, size_t> > > DictManager::dump_default_dict(
        bool reset, std::string root) {
        std::call_once(dump_dict_once_flag, [&]() {
            _result = dump_default_dict_internal(reset, root);
        });
        return _result;
    }

    turbo::Result<std::vector<std::pair<std::string, size_t> > > DictManager::dump_default_dict_internal(
        bool reset, std::string root) {
        _root = root;
        _reset = reset;

        if (_reset) {
            TURBO_UNUSED(turbo::remove_all(_root));
        }

        std::vector<std::pair<std::string, size_t> > info;
        bool r = true;
        auto rs = turbo::exists(_root);
        if (rs.ok() && rs.value_or_die()) {
            r = false;
        } else {
            r = true;
        }

        TURBO_RETURN_NOT_OK(dump_default_hadar_dict(r, info));
        TURBO_RETURN_NOT_OK(dump_default_xpinyin_dict(r, info));
        TURBO_RETURN_NOT_OK(dump_default_fasttext_dict(r, info));
        TURBO_RETURN_NOT_OK(dump_default_jieba_dict(r, info));

        return info;
    }

    turbo::Status DictManager::dump_default_fasttext_dict(bool reset, std::vector<std::pair<std::string,size_t>> &info) {
        auto fasttext_dict_path = turbo::FilePath(_root)/ "fasttext";
        auto write_root = fasttext_dict_path.string();
        auto ft_dict = gnef::dict::xxd_gen_files();
        if (ft_dict.empty()) {
            return turbo::invalid_argument_error("fasttext default configs is empty, but it need default config");
        }
        _fasttext_dict = (turbo::FilePath(fasttext_dict_path)/ft_dict[0].first).parent_path().string();
        if (!reset) {
            return turbo::OkStatus();
        }
        for (auto it: ft_dict) {
            TURBO_MOVE_OR_RAISE(auto r, dump_dict(write_root, it.first, it.second));
            info.push_back(r);
        }
        return turbo::OkStatus();
    }

    turbo::Status DictManager::dump_default_jieba_dict(bool reset, std::vector<std::pair<std::string,size_t>> &info) {
        auto jieba_dict_path = turbo::FilePath(_root)/ "jieba";
        auto write_root = jieba_dict_path.string();
        auto jieba_dicts = jieba::dict::xxd_gen_files();
        if (jieba_dicts.empty()) {
            return turbo::invalid_argument_error("jieba default configs is empty, but it need default config");
        }
        _jieba_dict = (turbo::FilePath(jieba_dict_path)/jieba_dicts[0].first).parent_path().string();
        if (!reset) {
            return turbo::OkStatus();
        }
        for (auto it: jieba_dicts) {
            TURBO_MOVE_OR_RAISE(auto r, dump_dict(write_root, it.first, it.second));
            info.push_back(r);
        }
        return turbo::OkStatus();
    }

    turbo::Status DictManager::dump_default_xpinyin_dict(bool reset, std::vector<std::pair<std::string,size_t>> &info) {
        auto xpinyin_dict_path = turbo::FilePath(_root)/ "xpinyin";
        auto write_root = xpinyin_dict_path.string();
        auto xpinyin_dicts = xpinyin::dict::xxd_gen_files();
        if (xpinyin_dicts.empty()) {
            return turbo::invalid_argument_error("xpinyin default configs is empty, but it need default config");
        }
        /// xpinyin need has two level
        _xpinyin_dict = (turbo::FilePath(xpinyin_dict_path)/xpinyin_dicts[0].first).parent_path().parent_path().string();
        if (!reset) {
            return turbo::OkStatus();
        }
        for (auto it: xpinyin_dicts) {
            TURBO_MOVE_OR_RAISE(auto r, dump_dict(write_root, it.first, it.second));
            info.push_back(r);
        }
        return turbo::OkStatus();
    }

    turbo::Status DictManager::dump_default_hadar_dict(bool reset, std::vector<std::pair<std::string,size_t>> &info) {
        auto hadar_dict_path = turbo::FilePath(_root)/ "hadar";
        auto write_root = hadar_dict_path.string();
        auto hadar_dicts = hadar::dict::xxd_gen_files();
        if (hadar_dicts.empty()) {
            return turbo::invalid_argument_error("hadar default configs is empty, but it need default config");
        }
        /// xpinyin need has two level
        _hadar_dict = (turbo::FilePath(hadar_dict_path)/hadar_dicts[0].first).parent_path().string();
        if (!reset) {
            return turbo::OkStatus();
        }
        for (auto it: hadar_dicts) {
            TURBO_MOVE_OR_RAISE(auto r, dump_dict(write_root, it.first, it.second));
            info.push_back(r);
        }
        return turbo::OkStatus();
    }
    std::string DictManager::hadar_dict() const {
        auto c = GnefConfig::instance().get_dict_config();
        if (!c->hadar_dict.empty()) {
            return c->hadar_dict;
        }

        return _hadar_dict;
    }

    std::string DictManager::xpinyin_dict() const {
        auto c = GnefConfig::instance().get_dict_config();
        if (!c->xpinyin_dict.empty()) {
            return c->xpinyin_dict;
        }

        return _xpinyin_dict;
    }

    std::string DictManager::fasttext_dict() const {
        auto c = GnefConfig::instance().get_dict_config();
        if (!c->fasttext_dict.empty()) {
            return c->fasttext_dict;
        }

        return _fasttext_dict;
    }

    std::string DictManager::jieba_dict() const {
        auto c = GnefConfig::instance().get_dict_config();
        if (!c->jieba_dict.empty()) {
            return c->jieba_dict;
        }

        return _jieba_dict;
    }

    std::string DictManager::ngram_dict() const {
        auto c = GnefConfig::instance().get_dict_config();
        if (!c->ngram_dict.empty()) {
            return c->ngram_dict;
        }
        return _ngram_dict;
    }
    std::string DictManager::intent_dict() const {
        auto c = GnefConfig::instance().get_dict_config();
        if (!c->intent_dict.empty()) {
            return c->intent_dict;
        }
        return _intent_dict;
    }
    std::string DictManager::rewrite_dict() const {
        auto c = GnefConfig::instance().get_dict_config();
        if (!c->rewrite_dict.empty()) {
            return c->rewrite_dict;
        }
        return _rewrite_dict;
    }
    std::string DictManager::ner_dict() const {
        auto c = GnefConfig::instance().get_dict_config();
        if (!c->ner_dict.empty()) {
            return c->ner_dict;
        }
        return _ner_dict;
    }


    std::string DictManager::default_hadar_dict() const {
        return _hadar_dict;
    }

    std::string DictManager::default_xpinyin_dict() const {
        return _xpinyin_dict;
    }

    std::string DictManager::default_fasttext_dict() const {
        return _fasttext_dict;
    }
    std::string DictManager::default_jieba_dict() const {
        return _jieba_dict;
    }

    std::string DictManager::default_ner_dict() const {
        return _ner_dict;
    }
    std::string DictManager::default_intent_dict() const {
        return _intent_dict;
    }
    std::string DictManager::default_rewrite_dict() const {
        return _rewrite_dict;
    }
    std::string DictManager::default_ngram_dict() const {
        return _ngram_dict;
    }
} // namespace gnef::api
