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

#include <gnef/sql/config/detect.h>
#include <gnef/api/config.h>

namespace gnef::sql {
    void pragma_detect_config(goose::ClientContext &context, const goose::FunctionParameters &parameters) {
        auto nc = std::make_shared<gnef::api::DetectConfig>();
        auto c = gnef::api::GnefConfig::instance().get_detect_config();
        nc->unknown = c->unknown;
        nc->threshold = c->threshold;
        nc->model = c->model;
        nc->prefix = c->prefix;
        try {
            /// { to avoid variable using for other field.
            {
                auto &u = parameters.values[0];
                auto vu = u.GetValue<std::string>();
                if (!vu.empty()) {
                    nc->unknown = vu;
                }
            }
            {
                auto &t = parameters.values[1];
                auto vt = t.GetValue<float>();
                if (vt > 0.0 && vt < 1.0) {
                    nc->threshold = vt;
                }
            }
            {
                auto &m = parameters.values[2];
                auto vm = m.GetValue<std::string>();
                if (!vm.empty()) {
                    nc->model = vm;
                }
            }
            {
                auto &p = parameters.values[3];
                auto vp = p.GetValue<std::string>();
                if (!vp.empty()) {
                    nc->prefix = vp;
                }
            }
            gnef::api::GnefConfig::instance().set_detect_config(nc);
        } catch (...) {
        }
    }
}  // gnef::sql