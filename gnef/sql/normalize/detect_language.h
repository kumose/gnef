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

namespace gnef::sql {

    ///////////////////////////////////////////////////////////////////////
    /// function detect_language, usage
    /// one select detect_lang('I am lothar');
    /// two select detect_lang('I am lothar', 0.6);
    void detect_language(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    ///////////////////////////////////////
    /// detect lang as a table
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
}  // gnef::sql
