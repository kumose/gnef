// Copyright (C) Kumo inc. and its affiliates.
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

#include <gnef/api/api.h>
#include <gnef/api/initializer.h>

int main(int argc, char **argv) {
    kumo::nlp::Config config;
    auto rs = gnef::api::initialize_gnef(config, true);
    if (!rs.ok()) {
        std::cerr << rs.message() << std::endl;
        return 1;
    }
    std::string output;
    kumo::nlp::NormalizeSetting setting = gnef::api::default_setting();
    setting.set_zh_to_pin(true);
    setting.set_zh_to_pin_short(true);
    setting.set_lang_detect(true);
    setting.set_upper_to_lower(true);
    setting.set_punctuation_to_space(true);
    setting.mutable_convert()->Add("s2tw");
    setting.mutable_convert()->Add("sp2tw");
    gnef::api::normalize(argv[1], setting, output);
    std::cout << output << std::endl;
    auto j = gnef::api::SegmentorInstance::instance().static_get<gnef::api::JiebaHandler>();
    std::vector<kmjieba::Word> result;
    j->cut(argv[1],  result);
    std::cout << result << std::endl;
    result.clear();
    j->cut_for_search(argv[1],  result);
    std::cout << result << std::endl;
    std::vector<std::pair<std::string, std::string>> t;
    j->tag(argv[1],  t);
    for (auto i : t) {
        std::cout << i.first<< ":" << i.second << std::endl;
    }
    kumo::nlp::SegmentRequest req;
    req.set_query(argv[1]);
    req.mutable_setting()->set_enable_pos(true);
    kumo::nlp::SegmentResult res;
    gnef::api::segment(req, res);
    std::cout << res.ShortDebugString() << std::endl;
}