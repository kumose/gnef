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

#include <gnef/sql/normalize/pinyin.h>
#include <gnef/api/config.h>
#include <gnef/api/dict.h>
#include <gnef/instance/pinyin.h>
#include <turbo/strings/ascii.h>

namespace gnef::sql {

    inline xpinyin::ManTone::Style round_type(int32_t t) {
        if (t&xpinyin::ManTone::TONE) {
            return xpinyin::ManTone::TONE;
        }
        if (t&xpinyin::ManTone::TONE2) {
            return xpinyin::ManTone::TONE2;
        }
        if (t&xpinyin::ManTone::TONE3) {
            return xpinyin::ManTone::TONE3;
        }
        return xpinyin::ManTone::NORMAL;
    }
    void chinese_pinyin(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto n = args.data.size();
        auto processer = gnef::api::PinyinInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->hanzi_to_pinyin(input.GetString());
            return goose::StringVector::AddString(result, r.toStdStr());
        };

        auto func_two = [&](goose::string_t input, int32_t t) {

            auto ty = round_type(t);
            auto r = processer->hanzi_to_pinyin(input.GetString(), ty);
            return goose::StringVector::AddString(result, r.toStdStr());
        };


        switch (n) {
            case 1: {
                goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                    func_one);
                break;
            }
            case 2: {
                auto &t = args.data[1];
                goose::BinaryExecutor::Execute<goose::string_t, int32_t, goose::string_t>(
                    name_vector, t, result, args.size(),
                    func_two);
                break;
            }
            default:
                TURBO_UNREACHABLE();
        }
    }

    void chinese_pinyin_short(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::PinyinInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->hanzi_to_pinyin(input.GetString());
            std::string ret;
            auto v = r.toStdVector();
            for (auto &it : v) {
                if (!it.empty()) {
                    auto c = it[0];
                    if (turbo::ascii_isalpha(c)) {
                        ret += it[0];
                    }
                }
            }
            return goose::StringVector::AddString(result, ret);
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                    func_one);
    }

}  // namespace gnef::sql
