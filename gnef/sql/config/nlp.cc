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

#include <gnef/sql/config/nlp.h>
#include <gnef/api/config.h>
#include <merak/flatten.h>
#include <merak/flatten/flat_pb.h>
#include "merak/proto/json_to_pb.h"
#include "merak/proto/pb_to_json.h"

namespace gnef::sql {
    static const auto kLost = gnef::api::GnefConfig::instance().nlp_setting_slots();

    struct NlpConfigData : public goose::GlobalTableFunctionState {
        NlpConfigData() : offset(0) {
        }

        goose::vector<NlpConfigInformation> entries;
        idx_t offset;
    };

    void nlp_init(goose::ClientContext &context, std::unique_ptr<NlpConfigData> &result) {
        auto func = [](const kumo::nlp::NlpSetting *setting) {
            turbo::flat_hash_map<std::string, std::string> ret;
            if (!setting) {
                return ret;
            }
            merak::FlatContainerHandler<turbo::flat_hash_map<std::string, std::string> > handler(ret);
            merak::proto_message_to_flat(*setting, handler);
            return ret;
        };
        {
            static kumo::nlp::NlpSetting kFull = gnef::api::GnefConfig::full_setting();
            auto ret_map = func(&kFull);
            for (auto &it: ret_map) {
                NlpConfigInformation entity;
                entity.key = it.first;
                entity.value = it.second;
                entity.region = "full";
                result->entries.push_back(std::move(entity));
            }
        }

        {
            static kumo::nlp::NlpSetting kDefault = gnef::api::GnefConfig::default_setting();
            auto ret_map = func(&kDefault);
            for (auto &it: ret_map) {
                NlpConfigInformation entity;
                entity.key = it.first;
                entity.value = it.second;
                entity.region = "default";
                result->entries.push_back(std::move(entity));
            }
        }

        {
            auto slots = gnef::api::GnefConfig::instance().nlp_setting_slots();
            for (size_t i = 0; i < slots; i++) {
                auto c = gnef::api::GnefConfig::instance().get_nlp_setting(i);
                auto slt = std::to_string(i);
                auto ret_map = func(c.get());
                for (auto &it: ret_map) {
                    NlpConfigInformation entity;
                    entity.key = it.first;
                    entity.value = it.second;
                    entity.region = slt;
                    result->entries.push_back(std::move(entity));
                }
            }
        }
        {
            auto u = gnef::api::GnefConfig::instance().get_all_nlp_user_setting();
            for (auto &it: u) {
                auto ret_map = func(it.second.get());
                for (auto &eit: ret_map) {
                    NlpConfigInformation entity;
                    entity.key = eit.first;
                    entity.value = eit.second;
                    entity.region = it.first;
                    result->entries.push_back(std::move(entity));
                }
            }
        }

        std::sort(result->entries.begin(), result->entries.end(),
                  [](const NlpConfigInformation &a, const NlpConfigInformation &b) {
                      if (a.region != b.region) {
                          return a.region < b.region;
                      }
                      return a.key < b.key;
                  });
    }


    goose::unique_ptr<goose::GlobalTableFunctionState> nlp_config_init(
        goose::ClientContext &context, goose::TableFunctionInitInput &input) {
        auto result = goose::make_uniq<NlpConfigData>();
        nlp_init(context, result);
        return std::move(result);
    }

    goose::unique_ptr<goose::FunctionData> nlp_config_bind(goose::ClientContext &context,
                                                           goose::TableFunctionBindInput &input,
                                                           goose::vector<goose::LogicalType> &return_types,
                                                           goose::vector<goose::string> &names) {
        names.emplace_back("config_key");
        return_types.emplace_back(goose::LogicalType::VARCHAR);

        names.emplace_back("region");
        return_types.emplace_back(goose::LogicalType::VARCHAR);

        names.emplace_back("value");
        return_types.emplace_back(goose::LogicalType::VARCHAR);

        return nullptr;
    }

    void nlp_config_function(goose::ClientContext &context, goose::TableFunctionInput &data_p,
                             goose::DataChunk &output) {
        auto &data = data_p.global_state->Cast<NlpConfigData>();
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
            output.SetValue(0, count, goose::Value(entry.key));
            output.SetValue(1, count, goose::Value(entry.region));
            output.SetValue(2, count, goose::Value(entry.value));
            data.offset++;
            count++;
        }
        output.SetCardinality(count);
    }

    void nlp_config(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result) {
        static const auto kFull = gnef::api::GnefConfig::full_setting();
        static const auto kDefault = gnef::api::GnefConfig::default_setting();

        auto func_slot = [&](int32_t slot) {
            if (0 > slot || slot >= kLost) {
                return goose::StringVector::AddString(
                    result, turbo::str_format("__error:%d slot config not found, slot should be [0,%d]", slot,
                                              kLost - 1));
            }
            auto c = gnef::api::GnefConfig::instance().get_nlp_setting(slot);
            std::string json;
            auto rs = merak::proto_message_to_json(*c.get(), &json);
            if (!rs.ok()) {
                return goose::StringVector::AddString(
                    result, turbo::str_format("__error:%d slot config transfer to json error:%s", slot, rs.message()));
            }
            return goose::StringVector::AddString(
                result, json);
        };

        auto func_string = [&](goose::string_t conf) {
            std::string_view cf(conf.GetData(), conf.GetSize());
            if (cf.empty()) {
                return goose::StringVector::AddString(
                    result, turbo::str_format("__error:empty config not found"));
            }

            const kumo::nlp::NlpSetting *setting = nullptr;
            /// hold ptr no release for processing
            std::shared_ptr<kumo::nlp::NlpSetting> c;
            if (turbo::equals_ignore_case(cf, "full")) {
                setting = &kFull;
            } else if (turbo::equals_ignore_case(cf, "default")) {
                setting = &kDefault;
            } else {
                c = gnef::api::GnefConfig::instance().get_nlp_setting(cf);
                if (!c) {
                    return goose::StringVector::AddString(
                        result, turbo::str_format("__error:%s config not found", cf));
                }
                setting = c.get();
            }


            std::string json;
            auto rs = merak::proto_message_to_json(*setting, &json);
            if (!rs.ok()) {
                return goose::StringVector::AddString(
                    result, turbo::str_format("__error:%s config transfer to json error:%s", cf, rs.message()));
            }
            return goose::StringVector::AddString(
                result, json);
        };
        auto &data1 = args.data[0];
        auto type = data1.GetType().id();
        switch (type) {
            case goose::LogicalType::INTEGER: {
                goose::UnaryExecutor::Execute<int32_t, goose::string_t>(
                    data1, result, args.size(),
                    func_slot);
                break;
            }
            case goose::LogicalType::VARCHAR: {
                goose::UnaryExecutor::Execute<goose::string_t, goose::string_t>(
                    data1, result, args.size(),
                    func_string);
                break;
            }
            default:
                TURBO_UNREACHABLE();
        }
    }

    //////////////////////////////
    void pragma_nlp_config(goose::ClientContext &context, const goose::FunctionParameters &parameters) {

        try {
            kumo::nlp::NlpSetting nc;
            auto &u = parameters.values[1];
            auto vu = u.GetValue<std::string>();
            auto rs = merak::json_to_proto_message(vu, &nc);
            if (!rs.ok()) {
                throw std::logic_error(rs.message());
            }
            auto &param = parameters.values[0];
            auto type = param.type().id();
            switch (type) {
                case goose::LogicalType::INTEGER: {
                    auto slot = param.GetValue<int32_t>();
                    if (slot < 0 || slot >= kLost) {
                        throw std::logic_error(turbo::str_format(
                            "__error:%d slot config not found, slot should be [0,%d]", slot,
                            kLost - 1));
                    }
                    gnef::api::GnefConfig::instance().set_nlp_setting(nc, slot);
                    break;
                }
                case goose::LogicalType::VARCHAR: {
                    auto cf = param.GetValue<std::string>();
                    if (cf.empty()) {
                        throw std::logic_error(turbo::str_format("__error: config should not be empty"));
                    }

                    if (turbo::equals_ignore_case(cf, "full")) {
                        throw std::logic_error(
                            turbo::str_format("__error: %s config should that, `full` config is system reserved", cf));
                    }
                    if (turbo::equals_ignore_case(cf, "default")) {
                        throw std::logic_error(
                            turbo::str_format("__error: %s config should that, `default` config is system reserved",
                                              cf));
                    }
                    gnef::api::GnefConfig::instance().set_nlp_setting(nc, cf);
                    break;
                }
                default:
                    TURBO_UNREACHABLE();
            }
        } catch (...) {
            throw;
        }
    }

    void pragma_nlp_config_user(goose::ClientContext &context, const goose::FunctionParameters &parameters) {
        auto conf = parameters.values[0].GetValue<std::string>();
        auto key = parameters.values[1].GetValue<std::string>();
        auto value = parameters.values[2].GetValue<std::string>();

        auto c = gnef::api::GnefConfig::instance().get_nlp_setting(std::string_view(conf));
        if (!c) {
            throw std::logic_error(turbo::str_format("__error: %s config not found", conf.c_str()));
        }
        kumo::nlp::NlpSetting nc = *c.get();
        merak::FlatProto proto(nc);
        auto rs = proto.set(std::string_view(key), std::string_view(value));
        if (!rs.ok()) {
            throw std::logic_error(rs.message());
        }
        gnef::api::GnefConfig::instance().set_nlp_setting(nc, std::string_view(conf));
    }

    void pragma_nlp_config_slot(goose::ClientContext &context, const goose::FunctionParameters &parameters) {
        auto &slot_param = parameters.values[0];
        auto &key_param = parameters.values[1];
        auto &value_param = parameters.values[2];

        auto slot = slot_param.GetValue<int32_t>();
        if (slot < 0 || slot >= static_cast<int32_t>(kLost)) {
            throw std::logic_error(turbo::str_format(
                "__error:%d slot out of range, valid is [0,%d]", slot,
                static_cast<int>(kLost) - 1));
        }

        auto key = key_param.GetValue<std::string>();
        auto val = value_param.GetValue<std::string>();
        if (key.empty()) {
            throw std::logic_error(turbo::str_format("__error: config key must not be empty"));
        }

        auto c = gnef::api::GnefConfig::instance().get_nlp_setting(static_cast<int>(slot));
        if (!c) {
            throw std::logic_error(turbo::str_format("__error: slot %d config not found", slot));
        }
        kumo::nlp::NlpSetting nc = *c.get();
        merak::FlatProto proto(nc);
        auto rs = proto.set(std::string_view(key), std::string_view(val));
        if (!rs.ok()) {
            throw std::logic_error(rs.message());
        }
        gnef::api::GnefConfig::instance().set_nlp_setting(nc, static_cast<int>(slot));
    }
}
