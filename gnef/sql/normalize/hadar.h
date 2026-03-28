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
    /// one select convert_hk2s('舞林大会') -> "wu lin da hui";

    void convert_hk2s(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_hk2t(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_jp2t(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_s2hk(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_s2t(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);


    void convert_s2tw(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_s2twp(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_t2hk(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_t2jp(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_t2s(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_t2tw(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_tw2s(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_tw2sp(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);

    void convert_tw2t(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);
    void convert_hadar(goose::DataChunk &args, goose::ExpressionState &state, goose::Vector &result);
}  // namespace gnef::sql
