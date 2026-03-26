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
#include <goose/function/table/system_functions.h>

#include <goose/common/file_system.h>
#include <goose/function/function_set.h>
#include <goose/main/client_context.h>
#include <goose/main/database.h>
#include <goose/main/extension_helper.h>
#include <goose/common/serializer/binary_deserializer.h>
#include <goose/main/extension_install_info.h>


namespace gnef {
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


        goose::TableFunctionSet table_functions("normalize_config");
        table_functions.AddFunction(
            goose::TableFunction({}, detect_config_function, detect_config_bind, detect_config_init));
        loader.RegisterFunction(table_functions);


        loader.RegisterFunction(goose::PragmaFunction::PragmaCall("pragma_detect_config", pragma_detect_config,
                                                                  {
                                                                      goose::LogicalType::VARCHAR,
                                                                      goose::LogicalType::FLOAT,
                                                                      goose::LogicalType::VARCHAR,
                                                                      goose::LogicalType::VARCHAR
                                                                  }));
    }
} // namespace gnef
