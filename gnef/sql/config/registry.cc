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


#include <gnef/sql/registry.h>
#include <gnef/sql/config/registry.h>
#include <gnef/sql/config/detect.h>
#include <gnef/sql/config/normalize.h>
#include <gnef/sql/config/nlp.h>

namespace gnef::sql::internal {
    void load_detect_config(goose::ExtensionLoader &loader) {
        loader.RegisterFunction(goose::PragmaFunction::PragmaCall("pragma_detect_config", pragma_detect_config,
                                                                  {
                                                                      goose::LogicalType::VARCHAR,
                                                                      goose::LogicalType::FLOAT,
                                                                      goose::LogicalType::VARCHAR,
                                                                      goose::LogicalType::VARCHAR
                                                                  }));
    }

    void load_normalize_config(goose::ExtensionLoader &loader) {
        goose::TableFunctionSet table_functions("normalize_config");
        table_functions.AddFunction(
            goose::TableFunction({}, detect_config_function, detect_config_bind, detect_config_init));
        loader.RegisterFunction(table_functions);
    }

    void load_nlp_config(goose::ExtensionLoader &loader) {
        goose::TableFunctionSet table_functions("nlp_config_table");
        table_functions.AddFunction(
            goose::TableFunction({}, nlp_config_function, nlp_config_bind, nlp_config_init));
        loader.RegisterFunction(table_functions);

        ///////////////
        /// show_nlp_config
        goose::ScalarFunctionSet show_nlp_config_functions("show_nlp_config");
        show_nlp_config_functions.AddFunction(
            goose::ScalarFunction({goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                  gnef::sql::nlp_config));
        show_nlp_config_functions.AddFunction(
            goose::ScalarFunction({goose::LogicalType::INTEGER}, goose::LogicalType::VARCHAR,
                                  gnef::sql::nlp_config));
        loader.RegisterFunction(show_nlp_config_functions);

        ///////////////
        /// pragma setting

        loader.RegisterFunction(goose::PragmaFunction::PragmaCall("set_user_nlp_config", pragma_nlp_config_user,
                                                          {
                                                              goose::LogicalType::VARCHAR,
                                                              goose::LogicalType::VARCHAR,
                                                              goose::LogicalType::VARCHAR
                                                          }));
        loader.RegisterFunction(goose::PragmaFunction::PragmaCall("set_nlp_config", pragma_nlp_config,
                                                          {
                                                              goose::LogicalType::INTEGER,
                                                              goose::LogicalType::VARCHAR
                                                          }));
        loader.RegisterFunction(goose::PragmaFunction::PragmaCall("set_kv_nlp_config", pragma_nlp_config_slot,
                                                            {
                                                                goose::LogicalType::INTEGER,
                                                                goose::LogicalType::VARCHAR,
                                                                goose::LogicalType::VARCHAR
                                                            }));
        loader.RegisterFunction(goose::PragmaFunction::PragmaCall("set_slot_nlp_config", pragma_nlp_config_slot,
                                                          {
                                                              goose::LogicalType::INTEGER,
                                                              goose::LogicalType::VARCHAR,
                                                              goose::LogicalType::VARCHAR
                                                          }));

    }
} // gnef::sql::internal


namespace gnef::sql {
    void load_config(goose::ExtensionLoader &loader) {
        internal::load_detect_config(loader);
        internal::load_normalize_config(loader);
        internal::load_nlp_config(loader);
    }
} // gnef::sql
