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
        if (root.empty()) {
            _root = GnefConfig::kSystemDictDirectory;
        } else {
            _root = root;
        }
        _reset = reset;

        if (_reset) {
            TURBO_UNUSED(turbo::remove_all(_root));
        }

        _hadar_dict = _root + "/hadar/";
        _xpinyin_dict = _root + "/xpinyin/dict/";
        _fasttext_dict = _root + "/fasttext/";
        std::vector<std::pair<std::string, size_t> > info;
        auto rs = turbo::exists(_root);
        if (rs.ok() && rs.value_or_die()) {
            return info;
        }


        auto ha_dict = hadar::dict::xxd_gen_files();
        for (auto it: ha_dict) {
            TURBO_MOVE_OR_RAISE(auto r, dump_dict(_hadar_dict, it.first, it.second));
            info.push_back(r);
        }
        auto xpinyin_dict = _root + "/xpinyin";
        auto xpin_dict = xpinyin::dict::xxd_gen_files();
        for (auto it: xpin_dict) {
            TURBO_MOVE_OR_RAISE(auto r, dump_dict(xpinyin_dict, it.first, it.second));
            info.push_back(r);
        }
        auto gnef_dict = gnef::dict::xxd_gen_files();
        for (auto it: gnef_dict) {
            TURBO_MOVE_OR_RAISE(auto r, dump_dict(_fasttext_dict, it.first, it.second));
            info.push_back(r);
        }
        return info;
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

    std::string DictManager::default_hadar_dict() const {
        return _hadar_dict;
    }

    std::string DictManager::default_xpinyin_dict() const {
        return _xpinyin_dict;
    }

    std::string DictManager::default_fasttext_dict() const {
        return _fasttext_dict;
    }
} // namespace gnef::api
