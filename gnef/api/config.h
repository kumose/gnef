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
#include <gnef/proto/search.pb.h>
#include  <turbo/utility/status.h>
#include <turbo/log/logging.h>

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

        using KVConfigType = taco::DoublyBufferedData<std::shared_ptr<turbo::flat_hash_map<std::string, std::string> > >
        ;
        using KVConfigTypePtr = taco::DoublyBufferedData<std::shared_ptr<turbo::flat_hash_map<std::string,
            std::string> > >::ScopedPtr;

        using NlpSettingType = taco::DoublyBufferedData<turbo::flat_hash_map<std::string, std::shared_ptr<kumo::nlp::NlpSetting>> >;
        using NlpSettingTypePtr = taco::DoublyBufferedData<turbo::flat_hash_map<std::string,
            std::shared_ptr<kumo::nlp::NlpSetting>> >::ScopedPtr;

        using NlpSlotSettingType = taco::DoublyBufferedData<std::shared_ptr<kumo::nlp::NlpSetting> >;
        using NlpSlotSettingTypePtr = taco::DoublyBufferedData<std::shared_ptr<kumo::nlp::NlpSetting> >::ScopedPtr;

    public:
        ~GnefConfig() = default;

        GnefConfig(const GnefConfig &) = delete;

        void operator=(const GnefConfig &) = delete;

        static GnefConfig &instance() {
            static GnefConfig ins;
            return ins;
        }

        std::shared_ptr<DictConfig> get_dict_config() const;

        void set_dict_config(std::shared_ptr<DictConfig> data);

        std::shared_ptr<DetectConfig> get_detect_config() const;

        void set_detect_config(std::shared_ptr<DetectConfig> data);

        std::shared_ptr<turbo::flat_hash_map<std::string, std::string> > get_kv_config() const;

        void set_kv_config(std::shared_ptr<turbo::flat_hash_map<std::string, std::string> > kv);

        turbo::Status load_pb_config(const kumo::nlp::Config &config);

        turbo::Status load_json_config(const std::string &json);

        template<typename T>
        static bool dict_modify_func(T &d, T new_data) {
            d = new_data;
            return true;
        }

        static bool modify_nlp_func(std::shared_ptr<kumo::nlp::NlpSetting> &d, const kumo::nlp::NlpSetting &new_data) {
            /// use double data ptr, avoid front end and backend race.
            auto ptr = std::make_shared<kumo::nlp::NlpSetting>(new_data);
            d = ptr;
            return true;
        }

        /// slot = 0 means default
        void set_nlp_setting(const kumo::nlp::NlpSetting &setting, int slot = 0) {
            switch (slot) {
                case 0:
                    _nlp_config_default.modify(modify_nlp_func, setting);
                    break;
                case 1:
                    _nlp_config_1.modify(modify_nlp_func, setting);
                    break;
                case 2:
                    _nlp_config_2.modify(modify_nlp_func, setting);
                    break;
                case 3:
                    _nlp_config_3.modify(modify_nlp_func, setting);
                    break;
                case 4:
                    _nlp_config_4.modify(modify_nlp_func, setting);
                    break;
                case 5:
                    _nlp_config_5.modify(modify_nlp_func, setting);
                    break;
                case 6:
                    _nlp_config_6.modify(modify_nlp_func, setting);
                    break;
                default:
                    break;
            }
        }


        std::shared_ptr<kumo::nlp::NlpSetting> get_nlp_setting(int slot = 0) {
            NlpSlotSettingTypePtr scope;
            switch (slot) {
                case 0: {
                    _nlp_config_default.read(&scope);
                    break;
                }
                case 1: {
                    _nlp_config_1.read(&scope);
                    break;
                }
                case 2: {
                    _nlp_config_2.read(&scope);
                    break;
                }
                case 3: {
                    _nlp_config_3.read(&scope);
                    break;
                }
                case 4: {
                    _nlp_config_4.read(&scope);
                    break;
                }
                case 5: {
                    _nlp_config_5.read(&scope);
                    break;
                }
                case 6: {
                    _nlp_config_6.read(&scope);
                    break;
                }
                default: {
                    KLOG_EVERY_MIN(ERROR)<<"the slot:["<<slot<<"] you called is out of range, using default config installed";
                    _nlp_config_default.read(&scope);
                    break;
                }
            }
            return *scope;
        }

        static bool modify_nlp_map_func(turbo::flat_hash_map<std::string, std::shared_ptr<kumo::nlp::NlpSetting> > &d,
                                std::string_view name, const kumo::nlp::NlpSetting &new_data) {
            /// use double data ptr, avoid front end and backend race.
            auto ptr = std::make_shared<kumo::nlp::NlpSetting>(new_data);
            d[name] = ptr;
            return true;
        }

        void set_nlp_setting(const kumo::nlp::NlpSetting &setting, std::string_view name) {
            _nlp_config.modify(modify_nlp_map_func, name, setting);
        }

        std::shared_ptr<kumo::nlp::NlpSetting> get_nlp_setting(std::string_view name) {
            NlpSettingTypePtr scoped;
            _nlp_config.read(&scoped);
            auto it = (*scoped).find(name);
            if (it != (*scoped).end()) {
                return it->second;
            }
            return nullptr;
        }

        static kumo::nlp::NlpSetting full_setting();

        static kumo::nlp::NlpSetting default_setting();

    private:
        GnefConfig();

        mutable DictConfigType _dict_config;
        mutable DetectConfigType _detect_config;
        mutable KVConfigType _kv_config;
        /// this is for user bug
        /// select nlp('气吞万里如虎','john_debug') or nlp('气吞万里如虎','kally_debug')
        mutable NlpSettingType _nlp_config;

        /// online application do not always change the config frequently,
        /// using slot for quick access,by select nlp('气吞万里如虎',1) or select nlp('气吞万里如虎',2)
        taco::DoublyBufferedData<std::shared_ptr<kumo::nlp::NlpSetting> > _nlp_config_default;
        taco::DoublyBufferedData<std::shared_ptr<kumo::nlp::NlpSetting> > _nlp_config_1;
        taco::DoublyBufferedData<std::shared_ptr<kumo::nlp::NlpSetting> > _nlp_config_2;
        taco::DoublyBufferedData<std::shared_ptr<kumo::nlp::NlpSetting> > _nlp_config_3;
        taco::DoublyBufferedData<std::shared_ptr<kumo::nlp::NlpSetting> > _nlp_config_4;
        taco::DoublyBufferedData<std::shared_ptr<kumo::nlp::NlpSetting> > _nlp_config_5;
        taco::DoublyBufferedData<std::shared_ptr<kumo::nlp::NlpSetting> > _nlp_config_6;
    };
} // namespace gnef::api
