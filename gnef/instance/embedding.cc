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


#include <gnef/instance/embedding.h>

namespace gnef::api {
    class NoopEmbeddingHandler : public Embedding {
    public:
        ~NoopEmbeddingHandler() override = default;

        std::string_view name() const override {
            return "noop_emb";
        }

        std::string_view arch() const override {
            return "noop_emb";
        }

        int dimensions() const override {
            return 0;
        }

        std::string_view description() const override {
            return "noop embedding";
        }

        turbo::Status inference(std::string_view sentence, std::vector<float> &result) const override {
            return turbo::OkStatus();
        }

        turbo::Status inference_batch(turbo::span<std::string_view> &sentence,
                                      std::vector<std::vector<float> > &result) const override {
            return turbo::OkStatus();
        }

        turbo::Status initialize(const std::string &dict_dir) override{
            return turbo::OkStatus();
        }
    };

    turbo::Status EmbeddingInstance::initialize(const std::string &dict_dir) {
        if (dict_dir.empty()) {
            return turbo::invalid_argument_error("dict dir is empty");
        }
        std::shared_ptr<NoopEmbeddingHandler> ptr;
        ptr.reset(new NoopEmbeddingHandler());
        TURBO_RETURN_NOT_OK(ptr->initialize(dict_dir));
        set(ptr);
        set_init();
        return turbo::OkStatus();
    }
} // gnef::api
