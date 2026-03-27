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

#include  <xpinyin/pinyin.h>

#include "turbo/utility/internal/status.h"

namespace gnef {
    class PinyinHandler {
    public:
        ~PinyinHandler();
        static PinyinHandler &instance() {
            static PinyinHandler instance;
            return instance;
        }

        static xpinyin::PinyinResVector hanzi_to_pinyin(const std::string &hans,
                              xpinyin::ManTone::Style style = xpinyin::ManTone::Style::NORMAL,
                              xpinyin::Error error = xpinyin::Default, bool candidates = true, bool v_to_u = false,
                              bool neutral_tone_with_five = false);

        turbo::Status initialize(const std::string &dict_dir = "");
    private:
        std::string _dict_path;
        std::unique_ptr<xpinyin::Pinyin> _pinyin;
    };
} // namespace genf
