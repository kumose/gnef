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

#include <gnef/sql/normalize/normalize.h>
#include <gnef/api/config.h>
#include <gnef/api/dict.h>
#include <gnef/instance/pinyin.h>
#include <turbo/strings/ascii.h>
#include <gnef/api/normalize.h>
#include <merak/protobuf.h>

namespace gnef::sql {
    struct DefaultSetting {
        static const kumo::nlp::NormalizeSetting &default_setting() {
            static const kumo::nlp::NormalizeSetting kDefaultSetting = gnef::api::default_setting();
            return kDefaultSetting;
        }

        static const std::string &default_json() {
            static const std::string json = []() {
                std::string result;
                TURBO_UNUSED(merak::proto_message_to_json(gnef::api::default_setting(), &result));
                return result;
            }();
            return json;
        }
    };


    void normalize_default(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto func_one = [&](goose::string_t input) {
            std::string json;
            gnef::api::normalize(input.GetString(), DefaultSetting::default_setting(), json);
            return goose::StringVector::AddString(result, json);
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void normalize_json(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto &jargs = args.data[1];
        auto func_two = [&](goose::string_t input, goose::string_t json) {
            kumo::nlp::NormalizeSetting setting;
            auto rs = merak::json_to_proto_message(json.GetString(), &setting);
            std::string ret;
            if (TURBO_LIKELY(rs.ok())) {
                gnef::api::normalize(input.GetString(), setting, ret);
            } else {
                gnef::api::normalize(input.GetString(), DefaultSetting::default_setting(), ret);
            }

            return goose::StringVector::AddString(result, ret);
        };
        goose::BinaryExecutor::Execute<goose::string_t, goose::string_t, goose::string_t>(
            name_vector, jargs, result, args.size(),
            func_two);
    }

    void normalize_default_setting(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto result_data = goose::FlatVector::GetData<goose::string_t>(result);
        for (size_t i = 0; i < args.size(); ++i) {
            result_data[i] = goose::StringVector::AddString(result, DefaultSetting::default_json());
        }
    }
} // namespace gnef::sql
