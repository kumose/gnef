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

#include <gnef/normalize/detect_language.h>
#include <fasttext/fasttext.h>
#include <gnef/instance/fasttext.h>

namespace gnef {
    void detect_language(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];

        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        [&](goose::string_t input) {
                                                                            auto const &text = input.GetString();
                                                                            std::vector<std::pair<float, std::string> >
                                                                                    predictions;
                                                                            std::istringstream ss(text);
                                                                            FastTextInstance::instance().bin().predictLine(
                                                                                ss, predictions, 1, 0.6);

                                                                            if (!predictions.empty()) {
                                                                                return goose::StringVector::AddString(
                                                                                    result, predictions[0].second);
                                                                            }
                                                                            return goose::StringVector::AddString(
                                                                                result, "unknown");
                                                                        });
    }

    void init_detect(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto &ftz_vector = args.data[1];
        goose::BinaryExecutor::Execute<goose::string_t, goose::string_t, goose::string_t>(name_vector, ftz_vector,result, args.size(),
                                                                        [&](goose::string_t input, goose::string_t input1) {
                                                                            FastTextInstance::init_call(input.GetString(), input1.GetString());
                                                                            return goose::StringVector::AddString(
                                                                                result, "ok");
                                                                        });
    }
} // namespace gnef
