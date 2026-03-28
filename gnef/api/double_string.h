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

#include <string>

namespace gnef::api {
    /// Optimizes memory usage by minimizing reallocations.
    /// Uses a dual-buffer swap strategy for multi-stage processing.
    /// Default (delta=0): Assumes processed size <= original size.
    /// Custom (delta>0): Allocates extra capacity based on the growth factor
    /// to accommodate expansive operations (e.g., Unicode normalization).
    class DoubleString {
    public:
        DoubleString() = default;

        ~DoubleString() = default;

        void set(const std::string &str, float delta = 0.0f, bool assign = false) {
            set_delta(str, delta, assign);
        }

        void set(std::string &&str, float delta = 0.0) {
            main_buffer = std::move(str);
            set_delta(main_buffer, delta, false);
        }

        void set(std::string_view str, float delta = 0.0, bool assign = false) {
            set_delta(str, delta, assign);
        }

        /// Pre-allocate buffers with a growth factor (delta) to prevent
        /// reallocations during expansive processing (e.g., Unicode normalization).
        void set_delta(std::string_view str, float delta, bool assign = false) {
            /// avoid utf32 missing last char
            size_t n = str.size() * delta + str.size() + 4;
            main_buffer.reserve(n);
            writer_buffer.reserve(n);
            if (assign) {
                main_buffer.assign(str);
                origin_view = main_buffer;
            } else {
                origin_view = str;
            }
        }

        /// call after one phrase, and entry
        /// next phrase using data() and buffer.
        /// return the buffer to write
        std::string * done() & noexcept {
            main_buffer.swap(writer_buffer);
            writer_buffer.clear();
            origin_view = main_buffer;
            return  &writer_buffer;
        }

        /// only call once, for final result
        const std::string &final() & noexcept {
            main_buffer.swap(writer_buffer);
            writer_buffer.clear();
            origin_view = main_buffer;
            return main_buffer;
        }

        /// only call once, for final result
        std::string final() && {
            main_buffer.swap(writer_buffer);
            writer_buffer.clear();
            origin_view = main_buffer;
            return std::move(main_buffer);
        }

        std::string &buffer() & {
            return writer_buffer;
        }

        std::string_view data() const {
            return origin_view;
        }

    private:
        std::string_view origin_view;
        std::string main_buffer;
        std::string writer_buffer;
    };
}
