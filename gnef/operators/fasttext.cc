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

#include <gnef/operators/fasttext.h>
#include <turbo/files/filesystem.h>

namespace gnef::api {

    turbo::Status FtzHandler::initialize(const std::string &dict_dir) {
        auto path = std::string(dict_dir) + "/lid.176.ftz";
        TURBO_MOVE_OR_RAISE(auto e, turbo::exists(path));
        if (!e) {
            return turbo::unavailable_error("dict not found, path: " + path);
        }
        _ftz.loadModel(path);
        return turbo::OkStatus();
    }

    // Custom zero-copy buffer to satisfy istream requirements
    struct FastMemBuf : std::streambuf {
        FastMemBuf(const char* base, size_t size) {
            char* p(const_cast<char*>(base));
            this->setg(p, p, p + size);
        }
    };

    std::vector<std::pair<float, std::string> >  FtzHandler::detect_language(std::string_view query, float threshold) {
        FastMemBuf buf(query.data(), query.size());
        std::istream ss(&buf);
        std::vector<std::pair<float, std::string> > predictions;

        _ftz.predictLine(
           ss, predictions, 1, threshold);
        return predictions;
    }
}  // gnef::api
