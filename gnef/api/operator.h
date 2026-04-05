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
#include <nlpproto/search.pb.h>
#include  <xpinyin/pinyin.h>
#include <turbo/container/span.h>

namespace gnef::api {

    /// we define the interface of nlp interface for
    /// operations. default, we implement the basics,
    /// but business more relevance module like intent
    /// rewrite, user need to implement it you self.
    /// the operator can be implement local call or remote call,
    /// but caution that, when you using remote call, it means that,
    /// care about the remote service stable. otherwise, if remote is
    /// un-access, guard that you have a local call for a base.
    class Segmenter {
    public:
        virtual ~Segmenter() = default;

        /////////////////////////////////////////////////////////////
        /// the parameter is the dirt dir
        virtual turbo::Status initialize(const std::string &dict_dir) = 0;

        virtual turbo::Status segment(const kumo::nlp::SegmentRequest &req, kumo::nlp::SegmentResult &res) const {
            return segment(req.query(), req.setting(), res);
        }

        virtual turbo::Status segment(std::string_view query, const kumo::nlp::SegmentSetting &setting, kumo::nlp::SegmentResult &res) const = 0;
    };

    class LangDetector {
    public:
        virtual ~LangDetector() = default;

        virtual std::vector<std::pair<float, std::string> >
        detect_language(std::string_view query, float threshold) = 0;;

        virtual turbo::Status initialize(const std::string &dict_dir) = 0;
    };

    /// using xpinyin as default
    /// data structure.
    class ConverPinyin {
    public:
        virtual ~ConverPinyin() = default;

        virtual turbo::Status initialize(const std::string &dict_dir) = 0;

        virtual xpinyin::PinyinResVector hanzi_to_pinyin(const std::string &hans,
                                                         xpinyin::ManTone::Style style =
                                                                 xpinyin::ManTone::Style::NORMAL,
                                                         xpinyin::Error error = xpinyin::Default,
                                                         bool candidates = true,
                                                         bool v_to_u = false,
                                                         bool neutral_tone_with_five = false) = 0;
    };

    class ConverComplex {
    public:
        virtual ~ConverComplex() = default;

        virtual turbo::Status initialize(const std::string &dict_dir) = 0;

        virtual turbo::Result<std::string> hk2s(const std::string &message) = 0;

        virtual turbo::Result<std::string> hk2t(const std::string &message) = 0;

        virtual turbo::Result<std::string> jp2t(const std::string &message) = 0;

        virtual turbo::Result<std::string> s2hk(const std::string &message) = 0;

        virtual turbo::Result<std::string> s2t(const std::string &message) = 0;


        virtual turbo::Result<std::string> s2tw(const std::string &message) = 0;

        virtual turbo::Result<std::string> s2twp(const std::string &message) = 0;

        virtual turbo::Result<std::string> t2hk(const std::string &message) = 0;

        virtual turbo::Result<std::string> t2jp(const std::string &message) = 0;

        virtual turbo::Result<std::string> t2s(const std::string &message) = 0;

        virtual turbo::Result<std::string> t2tw(const std::string &message) = 0;

        virtual turbo::Result<std::string> tw2s(const std::string &message) = 0;

        virtual turbo::Result<std::string> tw2sp(const std::string &message) = 0;

        virtual turbo::Result<std::string> tw2t(const std::string &message) = 0;

        virtual turbo::Result<std::string> convert(const std::string &message, const std::string &method) = 0;
    };

    class NerOps {
    public:
        virtual ~NerOps() = default;

        /// for that, no need to build a NerRequest pb object to improve performance
        virtual turbo::Status ner(const std::string &query, const kumo::nlp::NerSetting &setting, kumo::nlp::NerResponse &res) const = 0;
        virtual turbo::Status initialize(const std::string &dict_dir) = 0;
    };

    class RewriterOps {
    public:
        virtual ~RewriterOps() = default;

        /// the rewriter run phrase need all the informations before it run, so feed it, NlpResult,
        /// after running, move the result res to reqs rewrite field,but operator should not touch the
        /// req field to modify.
        virtual turbo::Status rewriter(const kumo::nlp::NlpResult &req, kumo::nlp::RewriteResult &res) const = 0;

        virtual turbo::Status initialize(const std::string &dict_dir) = 0;
    };

    /*
    class IntentOps {
    public:
        virtual ~IntentOps() = default;

        virtual turbo::Status intent(const kumo::nlp::NerRequest &req, kumo::nlp::NerResponse &res) const = 0;

        virtual turbo::Status initialize(const std::string &dict_dir) = 0;
    };
    */
} // namespace gnef::api
