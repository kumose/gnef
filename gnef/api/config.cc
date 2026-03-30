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

#include <gnef/api/config.h>
#include <merak/protobuf.h>

namespace gnef::api {

    const std::string GnefConfig::kSystemDictDirectory = "/tmp/gnefdict";

    static kumo::nlp::NlpSetting full_setting_internal() {
        kumo::nlp::NlpSetting setting;
        setting.set_name("kumo");
        auto m_nor = setting.mutable_normalize_setting();
        m_nor->set_full_to_half(true);
        m_nor->set_upper_to_lower(true);
        m_nor->set_blank_to_one(true);
        m_nor->set_lang_detect(true);
        m_nor->set_remove_bad_char(true);
        m_nor->set_remove_emoji(true);
        m_nor->set_punctuation_to_space(true);
        m_nor->set_remove_punctuation(true);
        m_nor->set_zh_to_pin(true);
        m_nor->set_zh_to_pin_short(true);
        m_nor->mutable_convert()->Add("s2hk");
        auto m_seg = setting.mutable_segment_setting();
        m_seg->set_enable_pos(true);
        m_seg->set_phrase(true);
        m_seg->set_limited(10);
        auto m_ner = setting.mutable_ner_setting();
        m_ner->set_pattern("match");
        m_ner->set_model("bert");
        m_ner->set_use_cache(true);
        m_ner->set_strategy("more");
        auto m_embedding = setting.mutable_embedding_setting();
        m_embedding->mutable_model()->Add("BGE");
        setting.mutable_ngram_setting()->set_ngram(4);
        setting.mutable_rewrite()->set_enable_correct(true);
        setting.mutable_rewrite()->set_enable_synonym(true);
        return setting;
    }

    static kumo::nlp::NlpSetting default_setting_internal() {
        kumo::nlp::NlpSetting setting;
        setting.set_name("gnef");
        auto m_nor = setting.mutable_normalize_setting();
        m_nor->set_full_to_half(true);
        //m_nor->set_upper_to_lower(true);
        m_nor->set_blank_to_one(true);
        m_nor->set_lang_detect(true);
        m_nor->set_remove_bad_char(true);
        m_nor->set_remove_emoji(true);
        m_nor->set_punctuation_to_space(true);
        m_nor->set_remove_punctuation(true);
        //m_nor->set_zh_to_pin(true);
        //m_nor->set_zh_to_pin_short(true);
        //m_nor->mutable_convert()->Add("s2hk");
        auto m_seg = setting.mutable_segment_setting();
        m_seg->set_enable_pos(true);
        //m_seg->set_phrase(true);
        m_seg->set_limited(10);
       // auto m_ner = setting.mutable_ner_setting();
       // m_ner->set_pattern("match");
       // m_ner->set_model("bert");
      //  m_ner->set_use_cache(true);
       // m_ner->set_strategy("more");
       // auto m_embedding = setting.mutable_embedding_setting();
       // m_embedding->mutable_model()->Add("BGE");
        //setting.mutable_ngram_setting()->set_ngram(4);
       // setting.mutable_rewrite()->set_enable_correct(true);
       // setting.mutable_rewrite()->set_enable_synonym(true);
        return setting;
    }

    kumo::nlp::NlpSetting GnefConfig::full_setting() {
        static const kumo::nlp::NlpSetting ins = full_setting_internal();
        return ins;
    }

    kumo::nlp::NlpSetting GnefConfig::default_setting() {
        static const kumo::nlp::NlpSetting ins = default_setting_internal();
        return ins;
    }
    std::shared_ptr<DictConfig> GnefConfig::get_dict_config() const {
        DictConfigTypePtr scope;
        _dict_config.read(&scope);
        return *scope;
    }

    void GnefConfig::set_dict_config(std::shared_ptr<DictConfig> data) {
        _dict_config.modify(dict_modify_func<std::shared_ptr<DictConfig>>, data);
    }

    std::shared_ptr<DetectConfig> GnefConfig::get_detect_config() const {
        DetectConfigTypePtr scope;
        _detect_config.read(&scope);
        return *scope;
    }

    void GnefConfig::set_detect_config(std::shared_ptr<DetectConfig> data) {
        _detect_config.modify(dict_modify_func<std::shared_ptr<DetectConfig>>, data);
    }

    std::shared_ptr<turbo::flat_hash_map<std::string, std::string>> GnefConfig::get_kv_config() const {
        KVConfigTypePtr scope;
        _kv_config.read(&scope);
        return *scope;
    }

    void GnefConfig::set_kv_config(std::shared_ptr<turbo::flat_hash_map<std::string, std::string>> kv) {
        _kv_config.modify(dict_modify_func<std::shared_ptr<turbo::flat_hash_map<std::string, std::string>>>, kv);
    }

    GnefConfig::GnefConfig() {
        auto dict = std::make_shared<DictConfig>();
        _dict_config.modify(dict_modify_func<std::shared_ptr<DictConfig>>, dict);
        auto detect = std::make_shared<DetectConfig>();
        _detect_config.modify(dict_modify_func<std::shared_ptr<DetectConfig>>, detect);
        auto kv = std::make_shared<turbo::flat_hash_map<std::string, std::string>>();
        _kv_config.modify(dict_modify_func<std::shared_ptr<turbo::flat_hash_map<std::string, std::string>>>, kv);
        auto d = default_setting_internal();
        set_nlp_setting(d, 0);
        set_nlp_setting(d, 1);
        set_nlp_setting(d, 2);
        set_nlp_setting(d, 3);
        set_nlp_setting(d, 4);
        set_nlp_setting(d, 5);
        set_nlp_setting(d, 6);
    }

    turbo::Status GnefConfig::load_pb_config(const kumo::nlp::Config &config) {
        auto dict = std::make_shared<DictConfig>();
        auto detect = std::make_shared<DetectConfig>();
        auto kv = std::make_shared<turbo::flat_hash_map<std::string, std::string>>();
        /// merge old
        *dict = *get_dict_config();
        *detect = *get_detect_config();
        *kv = *get_kv_config();
        /// merge new
        bool dict_changed = false;
        bool detect_changed = false;

        if (config.has_detect()) {
            auto d = config.detect();
            if (!d.unknown().empty()) {
                detect->unknown = d.unknown();
                detect_changed = true;
            }
            if (d.threshold() > 0.0 || d.threshold() < 1.0) {
                detect->threshold = d.threshold();
                detect_changed = true;
            }
            if (!d.model().empty()) {
                detect->model = d.model();
                detect_changed = true;
            }
            if (!d.prefix().empty()) {
                detect->prefix = d.prefix();
                detect_changed = true;
            }
        }

        if (config.has_dict()) {
            auto d = config.dict();
            if (!d.xpinyin_dict().empty()) {
                dict->xpinyin_dict = d.xpinyin_dict();
                dict_changed = true;
            }

            if (!d.fasttext_dict().empty()) {
                dict->fasttext_dict = d.fasttext_dict();
                dict_changed = true;
            }
            if (!d.jieba_dict().empty()) {
                dict->jieba_dict = d.jieba_dict();
                dict_changed = true;
            }

            if (!d.hadar_dict().empty()) {
                dict->hadar_dict = d.hadar_dict();
                dict_changed = true;
            }
        }

        for (auto &it : config.kv()) {
            (*kv)[it.key()] = it.value();
        }
        if (dict_changed) {
            _dict_config.modify(dict_modify_func<std::shared_ptr<DictConfig>>, dict);
        }

        if (detect_changed) {
            _detect_config.modify(dict_modify_func<std::shared_ptr<DetectConfig>>, detect);
        }
        if (!config.kv().empty()) {
            _kv_config.modify(dict_modify_func<std::shared_ptr<turbo::flat_hash_map<std::string, std::string>>>, kv);
        }
        return turbo::OkStatus();
    }

    turbo::Status GnefConfig::load_json_config(const std::string &json) {
        kumo::nlp::Config config;
        TURBO_RETURN_NOT_OK(merak::json_to_proto_message(json, &config));
        return load_pb_config(config);
    }


}  // namespace gnef::api
