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

#include <turbo/utility/status.h>
#include <gnef/api/operator.h>

namespace gnef::api {
    class NullNerOps : public NerOps {
    public:
        ~NullNerOps() override = default;

        /// for that, no need to build a NerRequest pb object to improve performance
        turbo::Status ner(const std::string &query, const kumo::nlp::NerSetting &setting,
                          kumo::nlp::NerResponse &res) const override;

        turbo::Status initialize(const std::string &dict_dir) override;

    private:
        friend class NerInstance;
        NullNerOps() = default;
    };
} // namespace gnef::api
