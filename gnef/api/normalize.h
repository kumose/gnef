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

#include <gnef/proto/normalize.pb.h>


namespace gnef::api {

    inline kumo::nlp::NormalizeSetting default_setting() {
        kumo::nlp::NormalizeSetting setting;
        setting.set_remove_emoji(true);
        setting.set_remove_bad_char(true);
        setting.set_blank_to_one(true);
        setting.set_full_to_half(true);
        return setting;
    }

    //////////////////////////////////////////////////////////////////////////
    /// flows
    /// 1. remove bad char ------------------- default
    /// 2. halfwidth trans ------------------- default
    /// 3. remove emoji    ------------------- default
    /// 4. remove prunt
    /// 5. punctuation_to_space
    /// 6. remove blank    ------------------- default
    /// 7. upper_to_lower
    /// 8. chinese to pinyin
    /// 9. lang detect
    void normalize(std::string_view input, const kumo::nlp::NormalizeSetting &setting,
                   kumo::nlp::NormalizeResponse &output);

    void normalize(std::string_view input, const kumo::nlp::NormalizeSetting &setting, std::string &json_output);

    template<bool remove_bad_char_flag>
    void to_halfwidth_fast(std::string_view input, std::string &output);

    void chinese_to_pinyin(const std::string &input, std::string &pinyin_output,
                           std::string *pinyin_short_output = nullptr);

    /// 1. halfwidth trans
    /// 2. normalize_remove
    std::string normalize_trans(std::string_view input, const kumo::nlp::NormalizeSetting &setting);

    /// 1. remove bad char
    /// 2. remove emoji
    /// 3. remove prunt
    /// 4. punctuation_to_space
    /// 5. remove blank
    /// 6. upper_to_lower
    template<bool remove_bad_char_flag>
    void normalize_remove(std::string_view input, const kumo::nlp::NormalizeSetting &setting, std::string &output);

    /// inlines
    /// @brief Detects and skips UTF-8 encoded Emoji characters.
    /// @param data Pointer to the input byte stream.
    /// @param len Total length of the input data.
    /// @param index Current scanning position.
    /// @param output Reference to the result buffer (DoubleString::buffer).
    /// @return The updated index after skipping the Emoji, or index + 1 if not an Emoji.
    ///
    /// @note In a single-pass scanner, this function effectively "erases" the
    /// Emoji by incrementing the index without pushing bytes to the output.
    /// @brief Detects and skips UTF-8 Emoji.
    /// @return True if an Emoji was skipped, False otherwise.
    inline bool remove_emoji(const char *__restrict data, size_t len, size_t &index) {
        unsigned char c = (unsigned char) data[index];

        /// 1. 4-byte Emoji (F0-F4)
        if (c >= 0xF0 && c <= 0xF4) {
            if (index + 3 < len) {
                index += 4;
                return true;
            }
        }

        /// 2. 3-byte Symbols (E2)
        if (c == 0xE2) {
            if (index + 2 < len) {
                index += 3;
                return true;
            }
        }

        /// 3. 3-byte Symbols (E3), skip if NOT Full-width space (E3 80)
        if (c == 0xE3) {
            if (index + 2 < len) {
                if ((unsigned char) data[index + 1] != 0x80) {
                    index += 3;
                    return true;
                }
            }
        }

        return false; // Not an Emoji, do nothing
    }

    /// @brief Detects and skips invalid UTF-8 sequences and control characters.
    /// @return The number of bytes to skip (0 if the character is "good").
    inline bool remove_bad_char(const char *data, size_t len, size_t &index) {
        unsigned char c = (unsigned char) data[index];

        /// 1. Handle ASCII Control Characters (0x00 - 0x1F)
        /// We allow \t(9), \n(10), \r(13), but reject others like \0, \v, \f.
        if (c < 0x20) {
            if (c == 9 || c == 10 || c == 13) return false; // Good
            ++index;
            return true; // Bad: skip 1 byte
        }

        /// 2. Handle DEL character (0x7F)
        if (c == 0x7F) {
            ++index;
            return true;
        }

        /// 3. Validate UTF-8 Multi-byte Sequences
        /// If it's a lead byte, ensure the trailing bytes are present.
        if (c >= 0xC2 && c <= 0xDF) {
            // 2-byte
            if (index + 1 >= len) {
                ++index;
                return true; // Incomplete
            }
            return false;
        }
        if (c >= 0xE0 && c <= 0xEF) {
            // 3-byte
            if (index + 2 >= len) {
                // Incomplete: skip remaining
                index = len;
                return true;
            }
            return false;
        }
        if (c >= 0xF0 && c <= 0xF4) {
            // 4-byte
            if (index + 3 >= len) {
                // Incomplete
                index = len;
                return true;
            }
            return false;
        }

        /// 4. Reject orphan trailing bytes (0x80 - 0xBF)
        /// A good UTF-8 sequence should never start with these.
        if (c >= 0x80 && c <= 0xBF) {
            ++index;
            return true;
        }

        return false; // Good character
    }
} // namespace gnef::api
