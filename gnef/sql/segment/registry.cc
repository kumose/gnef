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


#include <gnef/sql/segment/registry.h>
#include <gnef/sql/segment/segmenter.h>

namespace gnef::sql {

    void load_segment(goose::ExtensionLoader &loader) {
        goose::ScalarFunctionSet detect_lang("segment");
        /// one select detect_lang('I am lothar');
        detect_lang.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::segment));
        /// one select detect_lang('I am lothar', 0.6);
        detect_lang.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR, goose::LogicalType::BOOLEAN},
                                                      goose::LogicalType::VARCHAR,
                                                      gnef::sql::segment));

        loader.RegisterFunction(detect_lang);
    }

}  // gnef::sql
