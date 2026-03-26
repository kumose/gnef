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

#include <gnef/normalize/normalize.h>
#include <gnef/normalize/detect_language.h>
#include <goose/function/table_function.h>

namespace gnef {

    struct ShowConfigBindData : public goose::FunctionData {
        std::shared_ptr<DetectLanguage::ConfigSnapshot> snapshot;
        explicit ShowConfigBindData(std::shared_ptr<DetectLanguage::ConfigSnapshot> s)
            : snapshot(std::move(s)) {}
    };

    void load_normalize(goose::ExtensionLoader &loader) {
        goose::ScalarFunctionSet detect_lang("detect_lang");
        /// one select detect_lang('I am lothar');
        detect_lang.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::detect_language));
        /// one select detect_lang('I am lothar', 0.6);
        detect_lang.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR, goose::LogicalType::FLOAT},
                                                      goose::LogicalType::VARCHAR,
                                                      gnef::detect_language));
        /// one select detect_lang('I am lothar', 0.6, 'bin');
        detect_lang.AddFunction(goose::ScalarFunction(
            {goose::LogicalType::VARCHAR, goose::LogicalType::FLOAT, goose::LogicalType::VARCHAR},
            goose::LogicalType::VARCHAR,
            gnef::detect_language));

        loader.RegisterFunction(detect_lang);

        auto init_lang_detector = goose::ScalarFunction("init_detect",
                                                        {goose::LogicalType::VARCHAR, goose::LogicalType::VARCHAR},
                                                        goose::LogicalType::VARCHAR,
                                                        gnef::init_detect);
        loader.RegisterFunction(init_lang_detector);
    }
} // namespace gnef
