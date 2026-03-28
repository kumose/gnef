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

#include <gnef/api/initializer.h>
#include <gnef/api/config.h>
#include <gnef/api/dict.h>
#include <gnef/instance/pinyin.h>
#include <gnef/instance/hadar.h>
#include <gnef/instance/fasttext.h>
#include <merak/protobuf.h>
#include <turbo/files/file_util.h>

namespace gnef::api {

    std::atomic<size_t> init_version{0};

    turbo::Status initialize_gnef() {
        kumo::nlp::Config config;
        return initialize_gnef(config);
    }

    size_t initialize_version() {
        return init_version.load(std::memory_order_relaxed);
    }

    turbo::Status initialize_gnef(const kumo::nlp::Config &config) {
        /// init config
        TURBO_RETURN_NOT_OK(GnefConfig::instance().load_pb_config(config));

        /// dict
        std::string root;
        bool reset = false;
        if (config.has_dict()) {
            root = config.dict().system_dict();
            reset = config.dict().reset_system_dict();
        }
        TURBO_RETURN_NOT_OK(DictManager::instance().dump_default_dict(reset, root));

        /// instances
        TURBO_RETURN_NOT_OK(PinyinInstance::instance().initialize(DictManager::instance().xpinyin_dict()));

        TURBO_RETURN_NOT_OK(HadarInstance::instance().initialize(DictManager::instance().hadar_dict()));

        TURBO_RETURN_NOT_OK(FtzInstance::instance().initialize(DictManager::instance().fasttext_dict()));
        init_version.fetch_add(1, std::memory_order_acquire);
        return turbo::OkStatus();
    }

    turbo::Status initialize_gnef(const std::string &str) {
        kumo::nlp::Config config;
        auto rs = merak::json_to_proto_message(str, &config);
        if (rs.ok()) {
            return initialize_gnef(config);
        }
        config.Clear();
        if (config.ParseFromString(str)) {
            return initialize_gnef(config);
        }

        TURBO_RETURN_NOT_OK(turbo::exists(str));

        std::string context;
        TURBO_RETURN_NOT_OK(turbo::read_file_to_string(str, &context));
        config.Clear();
        rs = merak::json_to_proto_message(context, &config);
        if (rs.ok()) {
            return initialize_gnef(config);
        }
        config.Clear();
        if (config.ParseFromString(context)) {
            return initialize_gnef(config);
        }
        return turbo::invalid_argument_error("not known config type");
    }
}
