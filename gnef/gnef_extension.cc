// Copyright (C) DuckDB inc. and its affiliates.
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

#include <gnef/normalize/detect_language.h>

namespace goose {
    inline void QuackScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
        auto &name_vector = args.data[0];
        UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
            return StringVector::AddString(result, "Quack " + name.GetString() + " 🐥");
        });
    }

    inline void QuackOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
        auto &name_vector = args.data[0];
        UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
            return StringVector::AddString(result, "Quack " + name.GetString() + ", my linked OpenSSL version is " +
                                                   OPENSSL_VERSION_TEXT);
        });
    }

    static void LoadInternal(ExtensionLoader &loader) {
        // Register a scalar function
        auto quack_scalar_function = ScalarFunction("quack", {LogicalType::VARCHAR}, LogicalType::VARCHAR,
                                                    QuackScalarFun);
        loader.RegisterFunction(quack_scalar_function);

        auto detect_lang_function = ScalarFunction("detect_lang", {LogicalType::VARCHAR}, LogicalType::VARCHAR,
                                                    gnef::detect_language);
        loader.RegisterFunction(detect_lang_function);

        auto init_lang_detector = ScalarFunction("init_detect", {LogicalType::VARCHAR, LogicalType::VARCHAR}, LogicalType::VARCHAR,
                                                    gnef::init_detect);
        loader.RegisterFunction(init_lang_detector);

        // Register another scalar function
        auto quack_openssl_version_scalar_function = ScalarFunction("quack_openssl_version", {LogicalType::VARCHAR},
                                                                    LogicalType::VARCHAR, QuackOpenSSLVersionScalarFun);
        loader.RegisterFunction(quack_openssl_version_scalar_function);
    }

    void GnefExtension::Load(ExtensionLoader &loader) {
        LoadInternal(loader);
    }

    std::string GnefExtension::Name() {
        return "gnef";
    }

    std::string GnefExtension::Version() const {
#ifdef EXT_VERSION_QUACK
        return EXT_VERSION_QUACK;
#else
        return "";
#endif
    }
} // namespace duckdb

extern "C" {
GOOSE_CPP_EXTENSION_ENTRY(quack, loader) {
    goose::LoadInternal(loader);
}
}
