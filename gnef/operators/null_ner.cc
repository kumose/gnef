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

#include <gnef/operators/null_ner.h>

namespace gnef::api {
    turbo::Status NullNerOps::ner(const std::string &query, const kumo::nlp::NerSetting &setting,
                      kumo::nlp::NerResponse &res) const {
        return turbo::OkStatus();
    }

    turbo::Status NullNerOps::initialize(const std::string &dict_dir) {
        return turbo::OkStatus();
    }
} // namespace gnef::api
