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
    static goose::string_t process_detect(std::string_view query, goose::Vector &result, float threshold,
                                          std::string_view model, std::string_view prefix, std::string_view unknown) {
        std::vector<std::pair<float, std::string> >
                predictions = FastTextInstance::detect_language(query, threshold, model);

        if (!predictions.empty()) {
            auto data = turbo::strip_prefix(predictions[0].second, prefix);
            return goose::StringVector::AddString(
                result, goose::string_t(data.data(), data.size()));
        }
        return goose::StringVector::AddString(
            result, goose::string_t(unknown.data(), unknown.size()));
    }


    void detect_language(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto n = args.data.size();
        auto c = DetectLanguage::instance().get_snapshot();
        /// avoid pointer mutli addressing
        std::string_view unknown(c->unknown);
        std::string_view model(c->model);
        std::string_view prefix(c->prefix);
        float threshold = c->threshold;
        auto func_one = [&](goose::string_t input) {
            std::string_view text(input.GetData(), input.GetSize());
            return process_detect(text, result, threshold,
                                  model,prefix, unknown);
        };

        auto func_two = [&](goose::string_t input, float t) {
            std::string_view text(input.GetData(), input.GetSize());
            return process_detect(text, result, t,
                                  model, prefix, unknown);
        };
        auto func_three = [&](goose::string_t input, float t, goose::string_t m) {
            std::string_view text(input.GetData(), input.GetSize());
            model = std::string_view(m.GetData(), m.GetSize());
            return process_detect(text, result, t,
                                  model, prefix, unknown);
        };
        switch (n) {
            case 1: {
                goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                    func_one);
                break;
            }
            case 2: {
                auto &t = args.data[1];
                goose::BinaryExecutor::Execute<goose::string_t, float, goose::string_t>(
                    name_vector, t, result, args.size(),
                    func_two);
                break;
            }
            case 3: {
                auto &t = args.data[1];
                auto &m = args.data[2];
                goose::TernaryExecutor::Execute<goose::string_t, float, goose::string_t, goose::string_t>(
                    name_vector, t, m, result, args.size(),
                    func_three);
                break;
            }
            default:
                TURBO_UNREACHABLE();
        }

    }

    void init_detect(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto &ftz_vector = args.data[1];
        goose::BinaryExecutor::Execute<goose::string_t, goose::string_t, goose::string_t>(
            name_vector, ftz_vector, result, args.size(),
            [&](goose::string_t input, goose::string_t input1) {
                FastTextInstance::init_call(input.GetString(), input1.GetString());
                return goose::StringVector::AddString(
                    result, "ok");
            });
    }

} // namespace gnef
