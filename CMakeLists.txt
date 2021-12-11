cmake_minimum_required(VERSION 3.10)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(nmssearch)

add_executable(nmssearch src/main.cc src/nw_space.cc src/command_parser.cc src/build_index.cc src/align.cc src/sequence_reader.cc src/query_index.cc src/util.cc src/sequence_container.cc)

target_link_libraries(nmssearch "${PROJECT_SOURCE_DIR}/external/nmslib/similarity_search/release/libNonMetricSpaceLib.a")
target_include_directories(nmssearch PRIVATE "${PROJECT_SOURCE_DIR}/external/nmslib/similarity_search/include")

target_include_directories(nmssearch PRIVATE "${PROJECT_SOURCE_DIR}/external/cli11")

target_link_libraries(nmssearch "${PROJECT_SOURCE_DIR}/external/parasail/.libs/libparasail.a")
target_include_directories(nmssearch PRIVATE "${PROJECT_SOURCE_DIR}/external/parasail")

find_package(ZLIB)
target_link_libraries(nmssearch ZLIB::ZLIB)