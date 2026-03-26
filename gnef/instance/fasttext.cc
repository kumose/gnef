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

#include <gnef/instance/fasttext.h>
#include <mutex>
#include <sstream>
#include <build/gnef/dict/lid.176.ftz.xxd.h>
#include <gnef/dict/lid.176.ftz.xxd.h>
#include <gnef/dict/lid.176.bin.xxd.h>

namespace gnef {

    std::once_flag fasttext_init;

    void FastTextInstance::init_call_once(const std::string &bin, const std::string &ftz) {
        std::call_once(fasttext_init,[bin,ftz]() {
            FastTextInstance::instance().init(bin, ftz);
        });
    }

    void FastTextInstance::init_call(const std::string &bin, const std::string &ftz) {
        init_call_once(bin, ftz);
    }
    const fasttext::FastText &FastTextInstance::bin() const {
        init_call_once("", "");
        return _bin;
    }

    const fasttext::FastText &FastTextInstance::ftz() const {
        init_call_once("", "");
        return _ftz;
    }
    struct MemBuf : std::streambuf {
        MemBuf(const char* base, size_t size) {
            char* p(const_cast<char*>(base));
            this->setg(p, p, p + size);
        }
    };

    void FastTextInstance::init(const std::string &bin, const std::string &ftz) {
        std::cerr << "FastTextInstance::init start"<<std::endl;
        std::string tmp_bin = bin;
        if (bin.empty()) {
            tmp_bin = "/tmp/lid.176.bin";
            std::ofstream bin_file(tmp_bin, std::ios::binary | std::ios::trunc);
            bin_file.write(gnef::dict::dict_lid_176_bin.data(), gnef::dict::dict_lid_176_bin.size());
            bin_file.close();
        }

        _bin.loadModel(tmp_bin);
        std::string tmp_ftz = ftz;
        if (ftz.empty()) {
            tmp_ftz = "/tmp/lid.176.bin";
            std::ofstream ftz_file(tmp_ftz, std::ios::binary | std::ios::trunc);
            ftz_file.write(gnef::dict::dict_lid_176_ftz.data(), gnef::dict::dict_lid_176_ftz.size());
            ftz_file.close();
        }


        _ftz.loadModel(tmp_ftz);
        std::cerr << "FastTextInstance::init done" << std::endl;
    }

}  // namespace gnef
