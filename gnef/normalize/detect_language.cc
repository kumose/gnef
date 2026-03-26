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

    static std::vector<std::pair<float, std::string> > process_detect_table(std::string_view query, float threshold,
                                      std::string_view model) {
        std::vector<std::pair<float, std::string> >
                predictions = FastTextInstance::detect_language(query, threshold, model);
        return predictions;
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
                                  model, prefix, unknown);
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

    //////////////////////////////////////////////////////////
    goose::unique_ptr<goose::FunctionData> detect_config_bind(goose::ClientContext &context,
                                                              goose::TableFunctionBindInput &input,
                                                              goose::vector<goose::LogicalType> &return_types,
                                                              goose::vector<goose::string> &names) {
        names.emplace_back("config_name");
        return_types.emplace_back(goose::LogicalType::VARCHAR);

        names.emplace_back("region");
        return_types.emplace_back(goose::LogicalType::VARCHAR);

        names.emplace_back("type");
        return_types.emplace_back(goose::LogicalType::VARCHAR);

        names.emplace_back("reloaded");
        return_types.emplace_back(goose::LogicalType::BOOLEAN);

        names.emplace_back("value");
        return_types.emplace_back(goose::LogicalType::VARCHAR);

        names.emplace_back("description");
        return_types.emplace_back(goose::LogicalType::VARCHAR);

        return nullptr;
    }

    void detect_init(goose::ClientContext &context, std::unique_ptr<DetectConfigData> &result) {
        auto c = gnef::DetectLanguage::instance().get_snapshot();
        static gnef::DetectLanguage::ConfigSnapshot kDefaultDetectConfig;
        {
            ConfigInformation default_language;
            default_language.name = "unknown";
            default_language.region = "detect_language";
            default_language.type = "string";
            default_language.value = c->unknown;
            default_language.reloaded = c->unknown != kDefaultDetectConfig.unknown;
            default_language.description =
                    "The default string returned when language detection fails or confidence is below threshold";
            result->entries.push_back(std::move(default_language));
        }
        {
            ConfigInformation default_threshold;
            default_threshold.name = "threshold";
            default_threshold.region = "detect_language";
            default_threshold.type = "float";
            default_threshold.value = std::to_string(c->threshold);
            default_threshold.reloaded = c->threshold != kDefaultDetectConfig.threshold;
            default_threshold.description =
                    "The confidence score threshold (0.0-1.0). Results below this value are suppressed as 'unknown'";
            result->entries.push_back(std::move(default_threshold));
        }
        {
            ConfigInformation default_model;
            default_model.name = "model";
            default_model.region = "detect_language";
            default_model.type = "string";
            default_model.value = c->model;
            default_model.reloaded = c->model != kDefaultDetectConfig.model;
            default_model.description =
                    "The active FastText model identifier. 'bin' for high-precision (126MB), 'ftz' for compressed (~1MB)";
            result->entries.push_back(std::move(default_model));
        }
        {
            ConfigInformation default_prefix;
            default_prefix.name = "prefix";
            default_prefix.region = "detect_language";
            default_prefix.type = "string";
            default_prefix.value = c->prefix;
            default_prefix.reloaded = c->prefix != kDefaultDetectConfig.prefix;
            default_prefix.description =
                    "The internal label prefix to be stripped from FastText output (e.g., '__label__')";
            result->entries.push_back(std::move(default_prefix));
        }
    }

    goose::unique_ptr<goose::GlobalTableFunctionState> detect_config_init(
        goose::ClientContext &context, goose::TableFunctionInitInput &input) {
        auto result = goose::make_uniq<DetectConfigData>();
        detect_init(context, result);
        return result;
    }

    void detect_config_function(goose::ClientContext &context, goose::TableFunctionInput &data_p,
                                goose::DataChunk &output) {
        auto &data = data_p.global_state->Cast<DetectConfigData>();
        if (data.offset >= data.entries.size()) {
            // finished returning values
            return;
        }
        // start returning values
        // either fill up the chunk or return all the remaining columns
        idx_t count = 0;
        while (data.offset < data.entries.size() && count < STANDARD_VECTOR_SIZE) {
            auto &entry = data.entries[data.offset];

            // return values:
            // extension_name LogicalType::VARCHAR
            output.SetValue(0, count, goose::Value(entry.name));
            output.SetValue(1, count, goose::Value(entry.region));
            output.SetValue(2, count, goose::Value(entry.type));
            // loaded LogicalType::BOOLEAN
            output.SetValue(3, count, goose::Value::BOOLEAN(entry.reloaded));
            output.SetValue(4, count, goose::Value(entry.value));
            output.SetValue(5, count, goose::Value(entry.description));
            data.offset++;
            count++;
        }
        output.SetCardinality(count);
    }


    void pragma_detect_config(goose::ClientContext &context, const goose::FunctionParameters &parameters) {
        auto nc = std::make_shared<DetectLanguage::ConfigSnapshot>();
        auto c = DetectLanguage::instance().get_snapshot();
        nc->unknown = c->unknown;
        nc->threshold = c->threshold;
        nc->model = c->model;
        nc->prefix = c->prefix;
        try {
            /// { to avoid variable using for other field.
            {
                auto &u = parameters.values[0];
                auto vu = u.GetValue<std::string>();
                if (!vu.empty()) {
                    nc->unknown = vu;
                }
            }
            {
                auto &t = parameters.values[1];
                auto vt = t.GetValue<float>();
                if (vt > 0.0 && vt < 1.0) {
                    nc->threshold = vt;
                }
            }
            {
                auto &m = parameters.values[2];
                auto vm = m.GetValue<std::string>();
                if (!vm.empty()) {
                    nc->model = vm;
                }
            }
            {
                auto &p = parameters.values[3];
                auto vp = p.GetValue<std::string>();
                if (!vp.empty()) {
                    nc->prefix = vp;
                }
            }
            DetectLanguage::instance().set_snapshot(nc);
        } catch (...) {
        }
    }

    struct DetectTableData : public goose::TableFunctionData {
        DetectLanguage::ConfigSnapshot config;
        std::string query;
    };

    //////////////////////////////////////////////////////////
    /// detect table function
    goose::unique_ptr<goose::FunctionData> detect_lang_bind(goose::ClientContext &context,
                                                            goose::TableFunctionBindInput &input,
                                                            goose::vector<goose::LogicalType> &return_types,
                                                            goose::vector<goose::string> &names) {
        names.emplace_back("lang");
        return_types.emplace_back(goose::LogicalType::VARCHAR);

        names.emplace_back("probe");
        return_types.emplace_back(goose::LogicalType::FLOAT);
        auto ret = goose::make_uniq<DetectTableData>();
        ret->config = *DetectLanguage::instance().get_snapshot();
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

    struct DetectTableState : public goose::GlobalTableFunctionState {
        DetectTableState()   = default;
        std::vector<std::pair<float, std::string> > entries;
        int offset{0};
    };

    goose::unique_ptr<goose::GlobalTableFunctionState> detect_lang_init(
        goose::ClientContext &context, goose::TableFunctionInitInput &input) {
        auto result = goose::make_uniq<DetectTableState>();
        auto &qdata = input.bind_data->Cast<DetectTableData>();
        result->entries  = process_detect_table(qdata.query, qdata.config.threshold, qdata.config.model);
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
} // namespace gnef
