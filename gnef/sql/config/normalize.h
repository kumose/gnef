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

    struct ConfigInformation {
        std::string name;
        std::string region;
        std::string type;
        bool reloaded = false;;
        std::string value;
        std::string description;
    };

    goose::unique_ptr<goose::GlobalTableFunctionState> detect_config_init(goose::ClientContext &context,
                                                                          goose::TableFunctionInitInput &input);

    goose::unique_ptr<goose::FunctionData> detect_config_bind(goose::ClientContext &context,
                                                              goose::TableFunctionBindInput &input,
                                                              goose::vector<goose::LogicalType> &return_types,
                                                              goose::vector<goose::string> &names);

    void detect_config_function(goose::ClientContext &context, goose::TableFunctionInput &data_p,
                                goose::DataChunk &output);
}  // gnef::sql
