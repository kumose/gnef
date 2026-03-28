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

namespace gnef::api {

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
        return turbo::OkStatus();
    }

}
