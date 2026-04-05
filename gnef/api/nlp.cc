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

#include <gnef/api/nlp.h>
#include <gnef/api/normalize.h>
#include <gnef/api/segment.h>
#include <turbo/times/time.h>

namespace gnef::api {

    turbo::Status nlp_process(std::string_view query, const kumo::nlp::NlpSetting& setting, kumo::nlp::NlpResult& result) {
        turbo::TimeCost cost;
        result.set_raw_query(query);
        if (setting.enable_normalize()) {
            kumo::nlp::NormalizeResponse response;
            normalize(query,setting.normalize_setting(),response);
            *result.mutable_normalized() = std::move(response);
        }
        if (setting.enable_segment()) {
            kumo::nlp::SegmentResult response;
            auto rs = segment(query,setting.segment_setting(),response);
            if (!rs.ok()) {
                result.set_cost_us(cost.u_elapsed());
                return rs;
            }
            *result.mutable_terms() = std::move(response);
        }
        /*
        if (setting.enable_ner()) {

        }
        if (setting.enable_intent()) {

        }
        if (setting.enable_ngram()) {

        }
        if (setting.enable_rewrite()) {}
        */
        result.set_cost_us(cost.u_elapsed());
        return turbo::OkStatus();
    }
}  // namespace gnef::api
