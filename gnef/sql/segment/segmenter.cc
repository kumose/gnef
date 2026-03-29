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

#include <gnef/sql/segment/segmenter.h>
#include <gnef/api/segment.h>
#include <merak/protobuf.h>

namespace gnef::sql {
    void segment(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto n = args.data.size();

        auto func_one = [&](goose::string_t input) {
            kumo::nlp::SegmentRequest req;
            kumo::nlp::SegmentResult res;
            req.set_query(std::move(input.GetString()));
            std::string_view text(input.GetData(), input.GetSize());
            auto rs = gnef::api::segment(req, res);
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

        auto func_two = [&](goose::string_t input, bool pos) {
            kumo::nlp::SegmentRequest req;
            kumo::nlp::SegmentResult res;
            req.set_query(std::move(input.GetString()));
            req.mutable_setting()->set_enable_pos(pos);
            std::string_view text(input.GetData(), input.GetSize());
            auto rs = gnef::api::segment(req, res);
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
                goose::BinaryExecutor::Execute<goose::string_t, bool, goose::string_t>(
                    name_vector, pos, result, args.size(),
                    func_two);
                break;
            }
            default:
                TURBO_UNREACHABLE();
        }
    }
} // namespace gnef::sql
