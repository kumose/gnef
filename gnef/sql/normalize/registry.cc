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
#include <gnef/sql/normalize/pinyin.h>
#include <gnef/sql/normalize/hadar.h>

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

    void load_pinyin(goose::ExtensionLoader &loader) {
        goose::ScalarFunctionSet chinese_pinyin("chinese_pinyin");
        /// one select detect_lang('I am lothar');
        chinese_pinyin.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::chinese_pinyin));
        chinese_pinyin.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR, goose::LogicalType::INTEGER}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::chinese_pinyin));


        loader.RegisterFunction(chinese_pinyin);

        goose::ScalarFunctionSet chinese_pinyin_short("chinese_pinyin_short");
        chinese_pinyin_short.AddFunction(goose::ScalarFunction({goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::chinese_pinyin_short));
        loader.RegisterFunction(chinese_pinyin_short);



    }

    void load_convert (goose::ExtensionLoader &loader) {
        loader.RegisterFunction(goose::ScalarFunction("convert_hadar", {goose::LogicalType::VARCHAR, goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_hadar));
        loader.RegisterFunction(goose::ScalarFunction("convert_tw2t", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_tw2t));
        loader.RegisterFunction(goose::ScalarFunction("convert_tw2sp", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_tw2sp));
        loader.RegisterFunction(goose::ScalarFunction("convert_tw2s", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_tw2s));
        loader.RegisterFunction(goose::ScalarFunction("convert_t2tw", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_t2tw));
        loader.RegisterFunction(goose::ScalarFunction("convert_t2s", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_t2s));
        loader.RegisterFunction(goose::ScalarFunction("convert_t2jp", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_t2jp));
        loader.RegisterFunction(goose::ScalarFunction("convert_t2hk", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_t2hk));
        loader.RegisterFunction(goose::ScalarFunction("convert_s2twp", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_s2twp));
        loader.RegisterFunction(goose::ScalarFunction("convert_s2tw", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_s2tw));
        loader.RegisterFunction(goose::ScalarFunction("convert_s2t", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_s2t));
        loader.RegisterFunction(goose::ScalarFunction("convert_s2hk", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_s2hk));
        loader.RegisterFunction(goose::ScalarFunction("convert_jp2t", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_jp2t));
        loader.RegisterFunction(goose::ScalarFunction("convert_hk2t", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_hk2t));
        loader.RegisterFunction(goose::ScalarFunction("convert_hk2s", {goose::LogicalType::VARCHAR}, goose::LogicalType::VARCHAR,
                                                      gnef::sql::convert_hk2s));

    }
}  // namespace gnef::sql::internal



namespace gnef::sql {

    void load_normalize(goose::ExtensionLoader &loader) {
        /// load detect lang
        internal::load_detect_lang(loader);
        internal::load_pinyin(loader);
        internal::load_convert(loader);
    }
}  // gnef::sql
