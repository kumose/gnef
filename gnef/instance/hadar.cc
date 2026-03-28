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

#include <gnef/instance/hadar.h>
#include <turbo/strings/match.h>

namespace gnef::api {
    turbo::Status HadarHandler::initialize(const std::string &dict_path) {
        std::vector<std::string> roots = {dict_path, std::string(dict_path) + "/config/"};
        try {
            hk2s_ = std::make_unique<hadar::SimpleConverter>("hk2s.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount config ", "hk2s.json error ", e.what());
        }
        try {
            hk2t_ = std::make_unique<hadar::SimpleConverter>("hk2t.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "hk2t.json");
        }
        try {
            jp2t_ = std::make_unique<hadar::SimpleConverter>("jp2t.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "jp2t.json");
        }
        try {
            s2hk_ = std::make_unique<hadar::SimpleConverter>("s2hk.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "s2hk.json");
        }
        try {
            s2t_ = std::make_unique<hadar::SimpleConverter>("s2t.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "s2t.json");
        }
        try {
            s2tw_ = std::make_unique<hadar::SimpleConverter>("s2tw.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "s2tw.json");
        }
        try {
            s2twp_ = std::make_unique<hadar::SimpleConverter>("s2twp.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "s2twp.json");
        }
        try {
            t2hk_ = std::make_unique<hadar::SimpleConverter>("t2hk.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "t2hk.json");
        }
        try {
            t2jp_ = std::make_unique<hadar::SimpleConverter>("t2jp.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "t2jp.json");
        }
        try {
            t2s_ = std::make_unique<hadar::SimpleConverter>("t2s.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "t2s.json");
        }
        try {
            t2tw_ = std::make_unique<hadar::SimpleConverter>("t2tw.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "t2tw.json");
        }
        try {
            tw2s_ = std::make_unique<hadar::SimpleConverter>("tw2s.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "tw2s.json");
        }
        try {
            tw2sp_ = std::make_unique<hadar::SimpleConverter>("tw2sp.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "tw2sp.json");
        }
        try {
            tw2t_ = std::make_unique<hadar::SimpleConverter>("tw2t.json", roots);
        } catch (const std::exception &e) {
            return turbo::not_found_error("not fount %s config", "tw2t.json");
        }
        return turbo::OkStatus();
    }

    turbo::Result<std::string> HadarHandler::hk2s(const std::string &message) {
        try {
            return hk2s_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::hk2t(const std::string &message) {
        try {
            return hk2t_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::jp2t(const std::string &message) {
        try {
            return jp2t_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::s2hk(const std::string &message) {
        try {
            return s2hk_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::s2t(const std::string &message) {
        try {
            return s2t_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }


    turbo::Result<std::string> HadarHandler::s2tw(const std::string &message) {
        try {
            return s2tw_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::s2twp(const std::string &message) {
        try {
            return s2twp_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }


    turbo::Result<std::string> HadarHandler::t2hk(const std::string &message) {
        try {
            return t2hk_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::t2jp(const std::string &message) {
        try {
            return t2jp_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::t2s(const std::string &message) {
        try {
            return t2s_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::t2tw(const std::string &message) {
        try {
            return t2tw_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::tw2s(const std::string &message) {
        try {
            return tw2s_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::tw2sp(const std::string &message) {
        try {
            return tw2sp_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

    turbo::Result<std::string> HadarHandler::tw2t(const std::string &message) {
        try {
            return tw2t_->Convert(message);
        } catch (const std::exception &e) {
            return turbo::unavailable_error(e.what());
        }
    }

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

    turbo::Result<std::string> HadarHandler::convert(const std::string & message, const std::string & method) {
        auto lm = turbo::str_to_lower(method);
        static size_t hk2s_hash = std::hash<std::string>{}("hk2s");
        static size_t hk2t_hash = std::hash<std::string>{}("hk2t");
        static size_t jp2t_hash = std::hash<std::string>{}("jp2t");
        static size_t s2hk_hash = std::hash<std::string>{}("s2hk");
        static size_t s2t_hash = std::hash<std::string>{}("s2t");
        static size_t s2tw_hash = std::hash<std::string>{}("s2tw");
        static size_t s2twp_hash = std::hash<std::string>{}("s2twp");
        static size_t t2hk_hash = std::hash<std::string>{}("t2hk");
        static size_t t2jp_hash = std::hash<std::string>{}("t2jp");
        static size_t t2s_hash = std::hash<std::string>{}("t2s");
        static size_t t2tw_hash = std::hash<std::string>{}("t2tw");
        static size_t tw2s_hash = std::hash<std::string>{}("tw2s");
        static size_t tw2sp_hash = std::hash<std::string>{}("tw2sp");
        static size_t tw2t_hash = std::hash<std::string>{}("tw2t");

        auto qh = std::hash<std::string>{}("lm");
        if (lm == "hk2s") {
            return hk2s_->Convert(message);
        }
        if (lm == "hk2t") {
            return hk2t_->Convert(message);
        }
        if (lm == "jp2t") {
            return jp2t_->Convert(message);
        }
        if (lm == "s2hk") {
            return s2hk_->Convert(message);
        }
        if (lm == "s2t") {
            return s2t_->Convert(message);
        }
        if (lm == "s2tw") {
            return s2tw_->Convert(message);
        }
        if (lm == "s2twp") {
            return s2twp_->Convert(message);
        }
        if (lm == "t2hk") {
            return t2hk_->Convert(message);
        }
        if (lm == "t2jp") {
            return t2jp_->Convert(message);
        }
        if (lm == "t2s") {
            return t2s_->Convert(message);
        }
        if (lm == "t2tw") {
            return t2tw_->Convert(message);
        }
        if (lm == "tw2s") {
            return tw2s_->Convert(message);
        }
        if (lm == "tw2sp") {
            return tw2sp_->Convert(message);
        }
        if (lm == "tw2t") {
            return tw2t_->Convert(message);
        }
        return turbo::not_found_error("not found method");
    }

    turbo::Status HadarInstance::initialize(const std::string &dict_dir) {
        if (dict_dir.empty()) {
            return turbo::invalid_argument_error("dict dir is empty");
        }
        std::shared_ptr<HadarHandler> ptr;
        ptr.reset(new HadarHandler());
        TURBO_RETURN_NOT_OK(ptr->initialize(dict_dir));
        set(ptr);
        return turbo::OkStatus();
    }
} // namespace gnef::api
