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

#include <gnef/instance/complex_convert.h>

#include <gnef/sql/normalize/pinyin.h>
#include <gnef/api/config.h>
#include <gnef/api/dict.h>
#include <gnef/instance/pinyin_convert.h>
#include <turbo/strings/ascii.h>

namespace gnef::sql {

    void convert_hk2s(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->hk2s(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_hk2t(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->hk2t(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_jp2t(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->jp2t(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_s2hk(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->s2hk(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_s2t(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->s2t(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }


    void convert_s2tw(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->s2tw(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_s2twp(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->s2twp(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_t2hk(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->t2hk(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_t2jp(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->t2jp(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_t2s(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->t2s(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_t2tw(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->t2tw(input.GetString());
            if (!r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_tw2s(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->tw2s(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_tw2sp(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->tw2sp(input.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_tw2t(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_one = [&](goose::string_t input) {
            auto r = processer->tw2t(input.GetString());
            if (!r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(name_vector, result, args.size(),
                                                                        func_one);
    }

    void convert_hadar(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        auto &name_vector = args.data[0];
        auto &arg_vector = args.data[1];
        auto processer = gnef::api::ComplexConvertInstance::instance().get();
        auto func_two = [&](goose::string_t input, goose::string_t arg) {
            auto r = processer->convert(input.GetString(), arg.GetString());
            if (r.ok()) {
                return goose::StringVector::AddString(result, r.value_or_die());
            }
            return goose::StringVector::AddString(result, "__error:" + r.status().message());
        };
        goose::BinaryExecutor::Execute<goose::string_t, goose::string_t, goose::string_t>(name_vector, arg_vector, result, args.size(),
                                                                        func_two);
    }
} // namespace gnef::sql
