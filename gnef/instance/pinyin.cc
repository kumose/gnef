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
#include <gnef/instance/pinyin.h>
#include <turbo/random/rand_util.h>
#include <turbo/files/filesystem.h>
#include <xpinyin/dump_dict.h>
#include <xpinyin/g2p_global.h>

namespace gnef::api {


    turbo::Status PinyinHandler::initialize(const std::string &dict_dir) {
        _dict_path = dict_dir;
        xpinyin::setDictionaryPath(_dict_path);
        _pinyin = std::make_unique<xpinyin::Pinyin>();
        if (!xpinyin::is_loaded_dict()) {
            return turbo::unavailable_error("xpinyin load dict failed");
        }
        return turbo::OkStatus();
    }
    xpinyin::PinyinResVector PinyinHandler::hanzi_to_pinyin(const std::string &hans,
                              xpinyin::ManTone::Style style,
                              xpinyin::Error error, bool candidates, bool v_to_u,
                              bool neutral_tone_with_five) {
       return  _pinyin->hanziToPinyin(hans, style, error, candidates, v_to_u, neutral_tone_with_five);
    }

    turbo::Status PinyinInstance::initialize(const std::string &dict_dir) {
        if (dict_dir.empty()) {
            return turbo::invalid_argument_error("dict dir is empty");
        }

        auto ptr = std::make_shared<PinyinHandler>();
        TURBO_RETURN_NOT_OK(ptr->initialize(dict_dir));
        set(ptr);
        return turbo::OkStatus();
    }
}  // namespace gnef::api

