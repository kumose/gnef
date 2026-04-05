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

#include <nlpproto/search.pb.h>
#include <turbo/utility/status.h>

namespace gnef::api {

    turbo::Status nlp_process(std::string_view query, const kumo::nlp::NlpSetting& setting, kumo::nlp::NlpResult& result);

    inline turbo::Status nlp_process(const kumo::nlp::NlpRequest& request, kumo::nlp::NlpResult& result) {
        return  nlp_process(request.query(), request.setting(), result);
    }
}  // namespace gnef::api
