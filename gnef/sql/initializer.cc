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

#include <gnef/sql/initializer.h>
#include <gnef/api/initializer.h>

namespace gnef::sql {
    void initialize_version(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto version_item = std::to_string(gnef::api::initialize_version());
        auto result_data = goose::FlatVector::GetData<goose::string_t>(result);
        for (size_t i = 0; i < args.size(); ++i) {
            result_data[i] = goose::StringVector::AddString(result, version_item);
        }
    }
    void pragma_initialize_gnef(goose::ClientContext &context, const goose::FunctionParameters &parameters) {
        auto n = parameters.values.size();
        turbo::Status rs;
        if (n == 0) {
            rs = gnef::api::initialize_gnef();
            if (!rs.ok()) {
                throw goose::Exception(goose::ExceptionType::FATAL, turbo::str_format("initialize gnef error:%s", rs.message()));
            }

        } else {
            try {
                auto &u = parameters.values[0];
                auto vu = u.GetValue<std::string>();

                if (!vu.empty()) {
                    rs = gnef::api::initialize_gnef(vu);
                } else {
                    rs = gnef::api::initialize_gnef();
                }
                if (!rs.ok()) {
                    throw goose::Exception(goose::ExceptionType::FATAL, turbo::str_format("initialize gnef error:%s", rs.message()));
                }
            } catch (std::exception &e) {
                if (!rs.ok()) {
                    throw goose::Exception(goose::ExceptionType::FATAL, turbo::str_format("initialize gnef error:%s", e.what()));
                }
            }
        }
    }

}
