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

#include <gnef/api/normalize.h>
#include <gnef/api/double_string.h>
#include <turbo/strings/ascii.h>
#include <merak/protobuf.h>
#include <gnef/instance/pinyin.h>
#include <gnef/instance/fasttext.h>
#include <gnef/instance/hadar.h>

namespace gnef::api {

    void normalize(std::string_view input, const kumo::nlp::NormalizeSetting &setting, kumo::nlp::NormalizeResponse &output) {
        auto r = normalize_trans(input, setting);
        *output.mutable_query() = std::move(r);
        /// pinyin and pinyin short
        if (!setting.zh_to_pin() && !setting.zh_to_pin_short()) {
            return;
        }
        auto rv = PinyinInstance::instance().get()->hanzi_to_pinyin(output.query());
        if (setting.zh_to_pin()) {
            auto str = rv.toStdStr();
            *output.mutable_pinyin() = std::move(str);
        }

        if (setting.zh_to_pin_short()) {
            auto vp = rv.toStdVector();
            std::string sh;
            for (auto &v : vp) {
                if (v.empty() || turbo::ascii_isblank(v[0] )|| turbo::ascii_isspace(v[0])) {
                    continue;
                }
                sh.push_back(v[0]);
            }
            *output.mutable_pinyin_short() = std::move(sh);
        }
        if (setting.lang_detect()) {
            auto ins = FtzInstance::instance().get();
            auto r = ins->detect_language(output.query(), 0.6);
            if (!r.empty()) {
                output.set_lang(r[0].second);
                output.set_lang_probe(r[0].first);
            }
        }

        if (!setting.convert().empty()) {
            auto ins = HadarInstance::instance().get();
            auto ret = output.mutable_convert();
            ret->Reserve(setting.convert().size());
            for (auto &ct : setting.convert()) {
                auto r = ins->convert(output.query(),  ct);
                if (r.ok()) {
                    *ret->Add() = std::move(r).value_or_die();
                    continue;
                } else {
                    *ret->Add() = "__err:" + r.status().message();
                }
            }
        }
    }

    void normalize(std::string_view input, const kumo::nlp::NormalizeSetting &setting, std::string &json_output) {
        kumo::nlp::NormalizeResponse pb;
        normalize(input,setting, pb);
        TURBO_UNUSED(merak::proto_message_to_json(pb, &json_output));
    }

    void chinese_to_pinyin(const std::string &input, std::string &pinyin_output, std::string *pinyin_short_output) {

    }

    template<bool remove_bad_char_flag>
    void to_halfwidth_fast(std::string_view input, std::string &out) {
        out.reserve(input.size());

        for (size_t i = 0; i < input.size(); ++i) {
            unsigned char c1 = input[i];
            if constexpr (remove_bad_char_flag) {
                if (remove_bad_char(input.data(), input.size(), i)) {
                    continue;
                }
            }

            /// 1. Handle full-width space (UTF-8: E3 80 80)
            if (c1 == 0xE3 && i + 2 < input.size() &&
                (unsigned char) input[i + 1] == 0x80 && (unsigned char) input[i + 2] == 0x80) {
                out.push_back(' ');
                i += 2;
            }
            /// 2. Handle full-width character range (UTF-8: EF BC 81 to EF BD 9E)
            /// Full-width characters start with 'EF' prefix in UTF-8
            else if (c1 == 0xEF && i + 2 < input.size()) {
                unsigned char c2 = input[i + 1];
                unsigned char c3 = input[i + 2];

                /// Calculate Unicode codepoint value
                /// Unicode = (c1 & 0x0F) << 12 | (c2 & 0x3F) << 6 | (c3 & 0x3F)
                /// Full-width range U+FF01 - U+FF5E maps to UTF-8: EF BC 81 - EF BD 9E
                if (c2 == 0xBC && c3 >= 0x81 && c3 <= 0xBF) {
                    out.push_back((char) (c3 - 0x81 + 0x21));
                    i += 2;
                } else if (c2 == 0xBD && c3 >= 0x80 && c3 <= 0x9E) {
                    out.push_back((char) (c3 - 0x80 + 0x60));
                    i += 2;
                } else {
                    /// Keep other EF-prefixed characters unchanged
                    out.push_back(c1);
                }
            } else {
                // 3. Regular half-width or single-byte characters
                out.push_back(c1);
                if (c1 >= 0xE0 && c1 <= 0xEF && i + 2 < input.size()) {
                    out.push_back(input[++i]);
                    out.push_back(input[++i]);
                }
            }
        }
    }

    std::string normalize_trans(std::string_view input, const kumo::nlp::NormalizeSetting &setting) {
        DoubleString double_string;
        double_string.set(input);
        if (setting.full_to_half()) {
            if (setting.remove_bad_char()) {
                to_halfwidth_fast<true>(double_string.data(), double_string.buffer());
            } else {
                to_halfwidth_fast<false>(double_string.data(), double_string.buffer());
            }

            double_string.done();
            normalize_remove<false>(double_string.data(), setting, double_string.buffer());
            return std::move(double_string).final();
        }
        if (setting.remove_bad_char()) {
            normalize_remove<true>(double_string.data(), setting, double_string.buffer());
        } else {
            normalize_remove<false>(double_string.data(), setting, double_string.buffer());
        }

        return std::move(double_string).final();
    }

    template<bool remove_bad_char_flag>
    void normalize_remove(std::string_view input, const kumo::nlp::NormalizeSetting &setting, std::string &output) {
        auto data = input.data();
        auto len = input.size();
        /// avoid function call every time
        const bool need_remove_emoji = setting.remove_emoji();
        const bool need_remove_punctuation = setting.remove_punctuation();
        const bool need_punctuation_to_space = setting.punctuation_to_space();
        const bool need_blank_to_one = setting.blank_to_one();
        const bool need_upper_to_lower = setting.upper_to_lower();
        bool pre_is_spaced = false;
        for (size_t i = 0; i < len;) {
            unsigned char c1 = data[i];
            /// emoji
            if constexpr (remove_bad_char_flag) {
                if (remove_bad_char(data, len, i)) {
                    continue;
                }
            }


            if (need_remove_emoji && remove_emoji(data, len, i)) {
                continue;
            }
            if (need_remove_punctuation && turbo::ascii_ispunct(c1)) {
                ++i;
                continue;
            }


            auto is_blank = (turbo::ascii_isblank(c1) || turbo::ascii_isspace(c1));
            if (!need_remove_punctuation && need_punctuation_to_space && turbo::ascii_ispunct(c1)) {
                output.push_back(' ');
                ++i;
                pre_is_spaced = true;
                continue;
            }

            if (need_blank_to_one && pre_is_spaced && is_blank) {
                ++i;
                continue;
            }
            if (need_upper_to_lower && turbo::ascii_isupper(c1)) {
                output.push_back(turbo::ascii_tolower(c1));
                pre_is_spaced = false;
                ++i;
                continue;
            }

            output.push_back(c1);

            /// 3. Protect multi-byte integrity (Chinese characters, etc.)
            if (c1 >= 0xE0 && c1 <= 0xEF && i + 2 < len) {
                output.push_back(data[++i]);
                output.push_back(data[++i]);
            } else if (c1 >= 0xC2 && c1 <= 0xDF && i + 1 < len) {
                output.push_back(data[++i]);
            }

            ++i;
            pre_is_spaced = is_blank;
        }
    }

    template void to_halfwidth_fast<true>(std::string_view input, std::string &output);

    template void to_halfwidth_fast<false>(std::string_view input, std::string &output);

    template void normalize_remove<true>(std::string_view input, const kumo::nlp::NormalizeSetting &setting, std::string &output);
    template void normalize_remove<false>(std::string_view input, const kumo::nlp::NormalizeSetting &setting, std::string &output);
} // namespace gnef::api
