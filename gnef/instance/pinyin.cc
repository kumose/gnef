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

namespace gnef {


    std::once_flag pinyin_init;

    PinyinHandler::~PinyinHandler() {

        if (turbo::starts_with(_dict_path, "/tmp/temppinyindict/")) {
            TURBO_UNUSED(turbo::remove_all(_dict_path));
        }

    }
    void pinyin_init_func() {
        std::call_once(pinyin_init, [] () {
            TURBO_UNUSED(PinyinHandler::instance().initialize(""));
        });
    }
    turbo::Status PinyinHandler::initialize(const std::string &dict_dir) {
        if (!_dict_path.empty()) {
            return turbo::OkStatus();
        }
        auto dir = dict_dir;
        if (dir.empty()) {
            /// no dict, using own
            dir = std::string("/tmp/temppinyindict/") + std::to_string(turbo::RandUint64());
            auto rs = xpinyin::dump_dict(dir, true);
            std::cerr << rs<<std::endl;
            dir += "/dict/";
        }
        _dict_path = dir;
        xpinyin::setDictionaryPath(_dict_path);
        _pinyin = std::make_unique<xpinyin::Pinyin>();
        return turbo::OkStatus();
    }
    xpinyin::PinyinResVector PinyinHandler::hanzi_to_pinyin(const std::string &hans,
                              xpinyin::ManTone::Style style,
                              xpinyin::Error error, bool candidates, bool v_to_u,
                              bool neutral_tone_with_five) {
        pinyin_init_func();
       return  instance()._pinyin->hanziToPinyin(hans, style, error, candidates, v_to_u, neutral_tone_with_five);
    }
}  // namespace genf

