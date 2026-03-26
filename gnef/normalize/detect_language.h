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

#include <goose/goose.h>
#include <goose/common/exception.h>
#include <goose/function/scalar_function.h>
#include <goose/parser/parsed_data/create_scalar_function_info.h>
#include <shared_mutex>

namespace gnef {
    struct DetectLanguage {
        DetectLanguage() {
            std::unique_lock<std::shared_mutex> lock(_mutex);
            _config = std::make_shared<ConfigSnapshot>();
        }

        ~DetectLanguage() = default;

        static DetectLanguage &instance() {
            static DetectLanguage instance;
            return instance;
        }

        struct ConfigSnapshot {
            std::string unknown{"unknown"};
            float threshold{0.6};
            std::string model{"ftz"};
            std::string prefix{"__label__"};
        };

        std::shared_ptr<ConfigSnapshot> get_snapshot() const {
            std::shared_lock<std::shared_mutex> lock(_mutex);
            return _config;
        }

        void set_snapshot(std::shared_ptr<ConfigSnapshot> config) {
            std::unique_lock<std::shared_mutex> lock(_mutex);
            _config = config;
        }

        mutable std::shared_mutex _mutex;
        std::shared_ptr<ConfigSnapshot> _config;
    };

    void init_detect(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void detect_language(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    //////////////////////////////////////////
    /// config
    struct ConfigInformation {
        std::string name;
        std::string region;
        std::string type;
        bool reloaded = false;;
        std::string value;
        std::string description;
    };

    struct DetectConfigData : public goose::GlobalTableFunctionState {
        DetectConfigData() : offset(0) {
        }

        goose::vector<ConfigInformation> entries;
        idx_t offset;
    };

    goose::unique_ptr<goose::GlobalTableFunctionState> detect_config_init(goose::ClientContext &context,
                                                                          goose::TableFunctionInitInput &input);

    goose::unique_ptr<goose::FunctionData> detect_config_bind(goose::ClientContext &context,
                                                              goose::TableFunctionBindInput &input,
                                                              goose::vector<goose::LogicalType> &return_types,
                                                              goose::vector<goose::string> &names);

    void detect_config_function(goose::ClientContext &context, goose::TableFunctionInput &data_p,
                                goose::DataChunk &output);

    void pragma_detect_config(goose::ClientContext &context, const goose::FunctionParameters &parameters);

    //////////////////////////////////////////
    /// config
    struct DetectResult {
        std::string lang;
        float probe;
    };

    struct DetectResultData : public goose::GlobalTableFunctionState {
        DetectResultData() : offset(0) {
        }

        goose::vector<DetectResult> entries;
        idx_t offset;
    };

    goose::unique_ptr<goose::GlobalTableFunctionState> detect_lang_init(goose::ClientContext &context,
                                                                          goose::TableFunctionInitInput &input);

    goose::unique_ptr<goose::FunctionData> detect_lang_bind(goose::ClientContext &context,
                                                              goose::TableFunctionBindInput &input,
                                                              goose::vector<goose::LogicalType> &return_types,
                                                              goose::vector<goose::string> &names);

    void detect_lang_function(goose::ClientContext &context, goose::TableFunctionInput &data_p,
                                goose::DataChunk &output);

} // namespace gnef
