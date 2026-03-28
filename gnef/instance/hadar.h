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

#include <turbo/utility/status.h>
#include <gnef/instance/double_instance.h>
#include <hadar/hadar.h>
#include <hadar/simple_converter.h>

namespace gnef::api {

    class HadarHandler;

    class HadarInstance : public DoubleInstance<HadarHandler, HadarInstance> {
    public:
        ~HadarInstance() override = default;

        turbo::Status initialize(const std::string &dict_dir) override;

    private:
        friend class DoubleInstance<HadarHandler, HadarInstance>;
        HadarInstance() = default;
    };

    class HadarHandler {
    public:

       ~HadarHandler() = default;

        turbo::Status initialize(const std::string & dict_path);

        turbo::Result<std::string> hk2s(const std::string & message);

        turbo::Result<std::string> hk2t(const std::string & message);

        turbo::Result<std::string> jp2t(const std::string & message);

        turbo::Result<std::string> s2hk(const std::string & message);

        turbo::Result<std::string> s2t(const std::string & message);


        turbo::Result<std::string> s2tw(const std::string & message);

        turbo::Result<std::string> s2twp(const std::string & message);

        turbo::Result<std::string> t2hk(const std::string & message);

        turbo::Result<std::string> t2jp(const std::string & message);

        turbo::Result<std::string> t2s(const std::string & message);

        turbo::Result<std::string> t2tw(const std::string & message);

        turbo::Result<std::string> tw2s(const std::string & message);

        turbo::Result<std::string> tw2sp(const std::string & message);

        turbo::Result<std::string> tw2t(const std::string & message);

        turbo::Result<std::string> convert(const std::string & message, const std::string & method);
    private:
        friend class HadarInstance;
        friend std::shared_ptr<HadarHandler> std::make_shared<HadarHandler>();
        HadarHandler() = default;
    private:
        std::unique_ptr<hadar::SimpleConverter> hk2s_;
        std::unique_ptr<hadar::SimpleConverter> hk2t_;
        std::unique_ptr<hadar::SimpleConverter> jp2t_;
        std::unique_ptr<hadar::SimpleConverter> s2hk_;
        std::unique_ptr<hadar::SimpleConverter> s2t_;
        std::unique_ptr<hadar::SimpleConverter> s2tw_;
        std::unique_ptr<hadar::SimpleConverter> s2twp_;
        std::unique_ptr<hadar::SimpleConverter> t2hk_;
        std::unique_ptr<hadar::SimpleConverter> t2jp_;
        std::unique_ptr<hadar::SimpleConverter> t2s_;
        std::unique_ptr<hadar::SimpleConverter> t2tw_;
        std::unique_ptr<hadar::SimpleConverter> tw2s_;
        std::unique_ptr<hadar::SimpleConverter> tw2sp_;
        std::unique_ptr<hadar::SimpleConverter> tw2t_;
    };
} // namespace gnef::api
