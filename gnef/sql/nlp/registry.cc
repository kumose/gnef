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


#include <gnef/sql/nlp/registry.h>
#include <gnef/sql/nlp/process.h>

namespace gnef::sql {

    void load_nlp(goose::ExtensionLoader &loader) {
        goose::ScalarFunctionSet detect_lang("nlp_process");
        /// one select nlp_process('I am lothar');
        detect_lang.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::nlp_process));
        /// one select nlp_process('I am lothar', 1);
        detect_lang.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR, goose::LogicalType::INTEGER},
                                                      goose::LogicalType::VARCHAR,
                                                      gnef::sql::nlp_process));
        /// one select nlp_process('I am lothar', 'myconfig');
        detect_lang.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR, goose::LogicalType::VARCHAR},
                                                      goose::LogicalType::VARCHAR,
                                                      gnef::sql::nlp_process));

        loader.RegisterFunction(detect_lang);
    }

}  // gnef::sql
