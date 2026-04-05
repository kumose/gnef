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

#include <gnef/sql/nlp/process.h>
#include <gnef/api/nlp.h>
#include <merak/protobuf.h>
#include <gnef/api/config.h>

namespace gnef::sql {
    void nlp_process(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto n = args.data.size();

        auto func_one = [&](goose::string_t input) {
            kumo::nlp::NlpResult res;
            auto c = gnef::api::GnefConfig::instance().get_nlp_setting(0);
            auto rs = gnef::api::nlp_process(std::string_view(input.GetData(), input.GetSize()), *c.get(), res);
            if (!rs.ok()) {
                return goose::StringVector::AddString(
                    result, "execute fail");
            }
            std::string json;
            rs = merak::proto_message_to_json(res, &json);
            if (!rs.ok()) {
                return goose::StringVector::AddString(
                    result, "execute fail");
            }
            return goose::StringVector::AddString(
                result, json);
        };

        static const auto kLost = gnef::api::GnefConfig::instance().nlp_setting_slots();

        auto func_slot = [&](goose::string_t input, int32_t slot) {
            kumo::nlp::NlpResult res;

            if (slot < 0 || slot >= kLost) {
                slot = 0;
            }
            auto c = gnef::api::GnefConfig::instance().get_nlp_setting(slot);
            auto rs = gnef::api::nlp_process(std::string_view(input.GetData(), input.GetSize()), *c.get(), res);
            if (!rs.ok()) {
                return goose::StringVector::AddString(
                    result, "execute fail");
            }
            std::string json;
            rs = merak::proto_message_to_json(res, &json);
            if (!rs.ok()) {
                return goose::StringVector::AddString(
                    result, "execute fail");
            }
            return goose::StringVector::AddString(
                result, json);
        };

        auto func_user = [&](goose::string_t input, goose::string_t dict) {
            kumo::nlp::NlpResult res;
            auto c = gnef::api::GnefConfig::instance().
                    get_nlp_setting(std::string_view(dict.GetData(), dict.GetSize()));
            if (!c.get()) {
                return goose::StringVector::AddString(
                    result, "execute fail");
            }
            auto rs = gnef::api::nlp_process(std::string_view(input.GetData(), input.GetSize()), *c.get(), res);
            if (!rs.ok()) {
                return goose::StringVector::AddString(
                    result, "execute fail");
            }
            std::string json;
            rs = merak::proto_message_to_json(res, &json);
            if (!rs.ok()) {
                return goose::StringVector::AddString(
                    result, "execute fail");
            }
            return goose::StringVector::AddString(
                result, json);
        };
        switch (n) {
            case 1: {
                goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                    func_one);
                break;
            }
            case 2: {
                auto &pos = args.data[1];
                auto type = pos.GetType().id();
                switch (type) {
                    case goose::LogicalType::INTEGER: {
                        goose::BinaryExecutor::Execute<goose::string_t, int32_t, goose::string_t>(
                            name_vector, pos, result, args.size(),
                            func_slot);
                        break;
                    }
                    case goose::LogicalType::VARCHAR: {
                        goose::BinaryExecutor::Execute<goose::string_t, goose::string_t, goose::string_t>(
                            name_vector, pos, result, args.size(),
                            func_user);
                        break;
                    }
                        default:
                        TURBO_UNREACHABLE();
                }
                break;
            }
            default:
                TURBO_UNREACHABLE();
        }
    }
} // namespace gnef::sql
