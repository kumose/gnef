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

#include <gnef/gnef_extension.h>
#include <goose/goose.h>
#include <goose/common/exception.h>
#include <goose/function/scalar_function.h>
#include <goose/parser/parsed_data/create_scalar_function_info.h>
// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>
#include <gnef/version.h>
#include <gnef/sql/registry.h>

namespace goose {


    static void LoadInternal(ExtensionLoader &loader) {
        /// normalize
        gnef::sql::load_gnef(loader);
    }

    void GnefExtension::Load(ExtensionLoader &loader) {
        LoadInternal(loader);
    }

    std::string GnefExtension::Name() {
        return "gnef";
    }

    std::string GnefExtension::Version() const {
        static std::string version = std::string("v") + GNEF_VERSION_STRING;
        return version;
    }
} // namespace duckdb

extern "C" {
GOOSE_CPP_EXTENSION_ENTRY(quack, loader) {
    goose::LoadInternal(loader);
}
}
