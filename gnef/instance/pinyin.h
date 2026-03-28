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

#include <turbo/utility/status.h>
#include <gnef/instance/double_instance.h>

namespace gnef::api {

    class  PinyinHandler;

    class PinyinInstance : public DoubleInstance<PinyinHandler, PinyinInstance> {
    public:
        ~PinyinInstance() override = default;

        turbo::Status initialize(const std::string &dict_dir) override;

    private:
        friend class DoubleInstance<PinyinHandler, PinyinInstance>;
        PinyinInstance() = default;
    };

    class PinyinHandler {
    public:

        PinyinHandler() = default;
        ~PinyinHandler() = default;


        xpinyin::PinyinResVector hanzi_to_pinyin(const std::string &hans,
                                                        xpinyin::ManTone::Style style = xpinyin::ManTone::Style::NORMAL,
                                                        xpinyin::Error error = xpinyin::Default, bool candidates = true,
                                                        bool v_to_u = false,
                                                        bool neutral_tone_with_five = false);


    private:
        friend class PinyinInstance;

        turbo::Status initialize(const std::string & dict_dir);

    private:
        std::string _dict_path;
        std::unique_ptr<xpinyin::Pinyin> _pinyin;
    };
} // namespace gnef::api
