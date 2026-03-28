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

#include <gnef/sql/normalize/detect_language.h>
#include <gnef/api/config.h>
#include <gnef/api/dict.h>
#include <gnef/instance/fasttext.h>

namespace gnef::sql {


    inline goose::string_t process_detect(std::string_view query, goose::Vector &result, float threshold, std::string_view prefix, std::string_view unknown) {
        auto processer = gnef::api::FtzInstance::instance().get();
        std::vector<std::pair<float, std::string> >
                predictions = processer->detect_language(query, threshold);

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
        auto c = gnef::api::GnefConfig::instance().get_detect_config();
        /// avoid pointer mutli addressing
        std::string_view unknown(c->unknown);
        std::string_view prefix(c->prefix);
        float threshold = c->threshold;
        auto func_one = [&](goose::string_t input) {
            std::string_view text(input.GetData(), input.GetSize());
            return process_detect(text, result, threshold,prefix, unknown);
        };

        auto func_two = [&](goose::string_t input, float t) {
            std::string_view text(input.GetData(), input.GetSize());
            return process_detect(text, result, t, prefix, unknown);
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
            default:
                TURBO_UNREACHABLE();
        }
    }




    ////////////////////////////////////////////////////
    /// table functions
    inline  std::vector<std::pair<float, std::string> > process_detect_table(std::string_view query, float threshold) {
        auto processor = gnef::api::FtzInstance::instance().get();
        std::vector<std::pair<float, std::string> >
                predictions = processor->detect_language(query, threshold);
        return predictions;
    }

    struct DetectTableState : public goose::GlobalTableFunctionState {
        DetectTableState()   = default;
        std::vector<std::pair<float, std::string> > entries;
        int offset{0};
    };

    struct DetectTableData : public goose::TableFunctionData {
        gnef::api::DetectConfig config;
        std::string query;
    };

    goose::unique_ptr<goose::FunctionData> detect_lang_bind(goose::ClientContext &context,
                                                            goose::TableFunctionBindInput &input,
                                                            goose::vector<goose::LogicalType> &return_types,
                                                            goose::vector<goose::string> &names) {
        names.emplace_back("lang");
        return_types.emplace_back(goose::LogicalType::VARCHAR);

        names.emplace_back("probe");
        return_types.emplace_back(goose::LogicalType::FLOAT);
        auto ret = goose::make_uniq<DetectTableData>();
        ret->config = *gnef::api::GnefConfig::instance().get_detect_config();
        switch (input.inputs.size()) {
            case 4: {
                ret->config.prefix = input.inputs[3].GetValue<std::string>();
            }
            case 3: {
                ret->config.model = input.inputs[2].GetValue<std::string>();
            }
            case 2: {
                ret->config.threshold = input.inputs[1].GetValue<float>();
            }
            case 1: {
                ret->query = input.inputs[0].GetValue<std::string>();
                break;
            }
            default:
                TURBO_UNREACHABLE();
        }

        return ret;
    }

    goose::unique_ptr<goose::GlobalTableFunctionState> detect_lang_init(
    goose::ClientContext &context, goose::TableFunctionInitInput &input) {
        auto result = goose::make_uniq<DetectTableState>();
        auto &qdata = input.bind_data->Cast<DetectTableData>();
        result->entries  = process_detect_table(qdata.query, qdata.config.threshold);
        return result;
    }

    void detect_lang_function(goose::ClientContext &context, goose::TableFunctionInput &data_p,
                          goose::DataChunk &output) {
        auto &data = data_p.global_state->Cast<DetectTableState>();
        auto &bind_data = data_p.bind_data->Cast<DetectTableData>();
        if (data.offset >= data.entries.size()) {
            // finished returning values
            return;
        }

        idx_t count = 0;

        while (data.offset < data.entries.size() && count < STANDARD_VECTOR_SIZE) {
            auto &entry = data.entries[data.offset];
            auto lang = turbo::strip_prefix(entry.second, bind_data.config.prefix);
            output.SetValue(0, count, goose::Value(goose::string_t(lang.data(), lang.size())));
            output.SetValue(1, count, goose::Value(entry.first));
            ++data.offset;
            ++count;
        }

        output.SetCardinality(count);
    }
}  // gnef::sql
