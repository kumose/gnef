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

#include <fasttext/fasttext.h>

namespace gnef {
    class FastTextInstance {
    public:
        static FastTextInstance &instance() {
            static FastTextInstance instance;
            return instance;
        }

        const fasttext::FastText &bin() const;

        const fasttext::FastText &ftz() const;


        static void init_call(const std::string &bin, const std::string &ftz);

        static std::string dict_name() {
            return instance()._dict_bin;
        }

        static bool is_bin_enable() {
            return !instance()._dict_bin.empty();
        }

        /// query empty should be
        static std::vector<std::pair<float, std::string> >  detect_language(std::string_view query, float threshold, std::string_view model);
    private:
        FastTextInstance() = default;

        void init(const std::string &bin, const std::string &ftz);

        static void init_call_once(const std::string &bin, const std::string &ftz);

        bool _bin_initialized = false;
        std::string _dict_bin;
        mutable fasttext::FastText _bin;
        mutable fasttext::FastText _ftz;
    };
} // namespace gnef
