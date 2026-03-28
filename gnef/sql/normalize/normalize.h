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

#pragma once

#include <goose/goose.h>

namespace gnef::sql {

    ///////////////////////////////////////////////////////////////////////
    /// function chinese_pinyin, usage
    /// one select normalize('舞林大会☺') -> "舞林大会☺";
    /// two select normalize('武林外传') -> "wlwz";
    void normalize_default(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);
    void normalize_default_setting(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    //////////////////////////////////////////////////////////
    /// select normalize('武林外传', '{"full_to_half":true,"blank_to_one":true,"remove_bad_char":true,"remove_emoji":true, "remove_punctuation":true, "zh_to_pin":true}');
    void normalize_json(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);
}  // namespace gnef::sql
