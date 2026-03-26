# Copyright (C) Kumo inc. and its affiliates.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###########################################################################
# below are examples, after initialization, modify it to your own.
###########################################################################
###kmcmake_example

goose_cc_extension(
        NAMESPACE ${PROJECT_NAME}
        NAME gnef
        SOURCES
        normalize/detect_language.cc
        normalize/normalize.cc
        gnef_extension.cc
        instance/fasttext.cc
        DEPS
        goose_platform
        ABI_TYPE
        "CPP"
        CXXOPTS
        ${KMCMAKE_CXX_OPTIONS}
        PLINKS
        ${KMCMAKE_DEPS_LINK}
        PUBLIC
)
#[[
set(EXTENSION_CONFIG_BUILD ON)
include(extension_build_tools)

set(TARGET_NAME gnef)

set(EXTENSION_NAME ${TARGET_NAME}_extension)
set(LOADABLE_EXTENSION_NAME ${TARGET_NAME}_loadable_extension)
set(GOOSE_EXTENSION_GNEF_EXT_VERSION "0.1.0")
build_static_extension(${TARGET_NAME} gnef_extension.cc)
build_loadable_extension(${TARGET_NAME} " " gnef_extension.cc)

target_link_libraries(${EXTENSION_NAME}  ${KMCMAKE_DEPS_LINK})
target_link_libraries(${LOADABLE_EXTENSION_NAME}  ${KMCMAKE_DEPS_LINK})
]]

file(COPY ${PROJECT_SOURCE_DIR}/dict DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(DICT_FILES
        dict/lid.176.ftz
)

kmcmake_cc_xxd(
        NAME        gen_dict
        NAMESPACE   gnef::dict
        OUTDIR      ${CMAKE_CURRENT_BINARY_DIR}
        INCLUDEBASE ${PROJECT_BINARY_DIR}
        ASSETDIR    ${CMAKE_CURRENT_BINARY_DIR}
        FILES
        ${DICT_FILES}
)

kmcmake_cc_library(
        NAMESPACE ${PROJECT_NAME}
        NAME gnef_dict
        SOURCES
        ${gen_dict_SRCS}
        CXXOPTS
        ${KMCMAKE_CXX_OPTIONS}
        LINKS
        ${KMCMAKE_DEPS_LINK}
        PUBLIC
)

kmcmake_cc_binary(
        NAMESPACE ${PROJECT_NAME}
        NAME gnef
        SOURCES
        shell.cc
        CXXOPTS
        ${KMCMAKE_CXX_OPTIONS}
        LINKS
        goose::goose_shell_static
        goose::goose_static
        gnef::goose_gnef
        goose::autocomplete_static
        goose::vss_static
        goose::httpfs_static
        gnef::gnef_dict_static
        ${KMCMAKE_DEPS_LINK}
        LINKOPTS
        ${KMCMAKE_STATIC_BIN_OPTION}
)




###kmcmake_example