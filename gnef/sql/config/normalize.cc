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

#include <gnef/sql/config/normalize.h>
#include <gnef/api/config.h>

namespace gnef::sql {

    struct DetectConfigData : public goose::GlobalTableFunctionState {
        DetectConfigData() : offset(0) {
        }

        goose::vector<ConfigInformation> entries;
        idx_t offset;
    };

    void detect_init(goose::ClientContext &context, std::unique_ptr<DetectConfigData> &result) {
        auto c = gnef::api::GnefConfig::instance().get_detect_config();
        static gnef::api::DetectConfig kDefaultDetectConfig;
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
        return std::move(result);
    }

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
}