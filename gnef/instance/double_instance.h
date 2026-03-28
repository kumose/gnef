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

#include <taco/doubly_buffered_data.h>
#include <memory>
#include <turbo/utility/status.h>

namespace gnef::api {

    /// for reload able instance
    template<typename T, typename Sub>
    class DoubleInstance {
    public:
        virtual ~DoubleInstance() = default;

        static Sub &instance() {
            static Sub ins;
            return ins;
        }

        std::shared_ptr<T> get() {
            typename taco::DoublyBufferedData<std::shared_ptr<T> >::ScopedPtr reader;
            _data.read(&reader);
            return *reader;
        }

        static size_t template_set(std::shared_ptr<T> &ref, std::shared_ptr<T> data) {
            ref = data;
            return 1;
        }

        virtual turbo::Status initialize(const std::string & dict_dir) {
            return turbo::unimplemented_error("DoubleInstance::initialize");
        }
        void set(std::shared_ptr<T> ptr) {
            _data.modify(template_set, ptr);
        }
    private:
        taco::DoublyBufferedData<std::shared_ptr<T>> _data;
    };
} // namespace gnef::api
