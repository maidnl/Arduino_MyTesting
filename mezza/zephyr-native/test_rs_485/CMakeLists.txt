# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

# get value of NORMALIZED_BOARD_TARGET early
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE} COMPONENTS yaml boards)

set(DTC_OVERLAY_FILE ${CMAKE_CURRENT_LIST_DIR}/../../variants/${NORMALIZED_BOARD_TARGET}/${NORMALIZED_BOARD_TARGET}.overlay)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(test_rs_485)

target_include_directories(app 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Arduino/libraries/ArduinoRS485/src
)

target_sources(app PRIVATE src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Arduino/libraries/ArduinoRS485/src/RS485.cpp)

zephyr_compile_options(-Wno-unused-variable -Wno-comment)
