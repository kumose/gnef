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

#include <turbo/container/flat_hash_map.h>
#include <taco/doubly_buffered_data.h>
#include <gnef/proto/config.pb.h>
#include  <turbo/utility/status.h>

namespace gnef::api {

    struct DetectConfig {
        std::string unknown{"unknown"};
        float threshold{0.6};
        std::string model{"ftz"};
        std::string prefix{"__label__"};
    };

    struct DictConfig {
        /////////////////////
        /// dict path
        std::string xpinyin_dict;
        std::string fasttext_dict;
        std::string jieba_dict;
        std::string hadar_dict;
    };


    class GnefConfig {
    public:
        /// "/tmp/gnefdict"
        static const std::string kSystemDictDirectory;
    private:
        using DictConfigType = taco::DoublyBufferedData<std::shared_ptr<DictConfig> >;
        using DictConfigTypePtr = taco::DoublyBufferedData<std::shared_ptr<DictConfig> >::ScopedPtr;

        using DetectConfigType = taco::DoublyBufferedData<std::shared_ptr<DetectConfig> >;
        using DetectConfigTypePtr = taco::DoublyBufferedData<std::shared_ptr<DetectConfig> >::ScopedPtr;

        using KVConfigType = taco::DoublyBufferedData<std::shared_ptr<turbo::flat_hash_map<std::string, std::string>> >;
        using KVConfigTypePtr = taco::DoublyBufferedData<std::shared_ptr<turbo::flat_hash_map<std::string, std::string>> >::ScopedPtr;

    public:
        ~GnefConfig() = default;
        GnefConfig(const GnefConfig&) = delete;
        void operator=(const GnefConfig&) = delete;

        static GnefConfig &instance() {
            static GnefConfig ins;
            return ins;
        }

        std::shared_ptr<DictConfig> get_dict_config() const;

        void set_dict_config(std::shared_ptr<DictConfig> data);

        std::shared_ptr<DetectConfig> get_detect_config() const;

        void set_detect_config(std::shared_ptr<DetectConfig> data);

        std::shared_ptr<turbo::flat_hash_map<std::string, std::string>> get_kv_config() const;

        void set_kv_config(std::shared_ptr<turbo::flat_hash_map<std::string, std::string>> kv);

        turbo::Status load_pb_config(const kumo::nlp::Config &config);

        turbo::Status load_json_config(const std::string &json);

        template <typename T>
        static bool dict_modify_func(T &d, T new_data) {
            d = new_data;
            return true;
        }

    private:
        GnefConfig();

        mutable taco::DoublyBufferedData<std::shared_ptr<DictConfig> > _dict_config;
        mutable taco::DoublyBufferedData<std::shared_ptr<DetectConfig> > _detect_config;
        mutable taco::DoublyBufferedData<std::shared_ptr<turbo::flat_hash_map<std::string, std::string>> > _kv_config;
    };
} // namespace gnef::api
