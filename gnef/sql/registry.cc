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
#include <gnef/sql/normalize/registry.h>
#include <gnef/sql/config/registry.h>
#include <gnef/sql/dict/registry.h>
#include <gnef/sql/segment/registry.h>
#include <gnef/sql/nlp/registry.h>
#include <gnef/sql/initializer.h>

namespace gnef::sql {

    void load_gnef(goose::ExtensionLoader &loader) {
        load_dict(loader);
        load_config(loader);
        load_normalize(loader);
        internal::load_initializer(loader);
        load_segment(loader);
        load_nlp(loader);
    }

}  // gnef::sql

namespace gnef::sql::internal {
    void load_initializer(goose::ExtensionLoader &loader) {

        loader.RegisterFunction(goose::PragmaFunction::PragmaCall("initialize_gnef_default", gnef::sql::pragma_initialize_gnef,
                                                                 {
                                                                 }));

        loader.RegisterFunction(goose::PragmaFunction::PragmaCall("initialize_gnef", gnef::sql::pragma_initialize_gnef,
                                                                 {
                                                                     goose::LogicalType::VARCHAR
                                                                 }));
        /// one select initialize_version;
        loader.RegisterFunction(goose::ScalarFunction("initialize_version",{}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::initialize_version));

    }
}