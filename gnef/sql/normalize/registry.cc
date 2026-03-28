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

#include <gnef/sql/normalize/registry.h>
#include <gnef/sql/normalize/detect_language.h>

/// for internal call
namespace gnef::sql::internal {
    void load_detect_lang(goose::ExtensionLoader &loader) {
        goose::ScalarFunctionSet detect_lang("detect_lang");
        /// one select detect_lang('I am lothar');
        detect_lang.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::detect_language));
        /// one select detect_lang('I am lothar', 0.6);
        detect_lang.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR, goose::LogicalType::FLOAT},
                                                      goose::LogicalType::VARCHAR,
                                                      gnef::sql::detect_language));

        loader.RegisterFunction(detect_lang);

        goose::TableFunctionSet detect_table_functions("detect_lang_table");
        detect_table_functions.AddFunction(
            goose::TableFunction({goose::LogicalType::VARCHAR}, detect_lang_function, detect_lang_bind,
                                 detect_lang_init));
        detect_table_functions.AddFunction(
            goose::TableFunction({goose::LogicalType::VARCHAR, goose::LogicalType::FLOAT}, detect_lang_function,
                                 detect_lang_bind, detect_lang_init));
        detect_table_functions.AddFunction(
            goose::TableFunction({goose::LogicalType::VARCHAR, goose::LogicalType::FLOAT, goose::LogicalType::VARCHAR},
                                 detect_lang_function, detect_lang_bind, detect_lang_init));
        detect_table_functions.AddFunction(
            goose::TableFunction({
                                     goose::LogicalType::VARCHAR, goose::LogicalType::FLOAT,
                                     goose::LogicalType::VARCHAR, goose::LogicalType::VARCHAR
                                 }, detect_lang_function, detect_lang_bind, detect_lang_init));
        loader.RegisterFunction(detect_table_functions);

    }
}  // namespace gnef::sql::internal



namespace gnef::sql {

    void load_normalize(goose::ExtensionLoader &loader) {
        /// load detect lang
        internal::load_detect_lang(loader);
    }
}  // gnef::sql
