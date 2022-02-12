cmake_minimum_required(VERSION 3.10)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(WITH_OPENMP OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory("external/parasail")
add_subdirectory("external/nmslib/similarity_search")

project(nmssearch)

# Static library
add_library(libnmssearch STATIC
    src/nw_space.cc 
    src/command_parser.cc 
    src/build_index.cc 
    src/dump_index.cc
    src/align.cc 
    src/sequence_reader.cc 
    src/query_index.cc 
    src/sequence_container.cc 
    src/output_writer.cc)
set_target_properties(libnmssearch PROPERTIES OUTPUT_NAME "nmssearch")

# For NMSLIB we have to set the include path manually, since it is not specified in the library CMakeLists.txt
target_link_libraries(libnmssearch PUBLIC NonMetricSpaceLib)
target_include_directories(libnmssearch PRIVATE "${PROJECT_SOURCE_DIR}/external/nmslib/similarity_search/include")

target_include_directories(libnmssearch PRIVATE "${PROJECT_SOURCE_DIR}/external/cli11")

target_link_libraries(libnmssearch PUBLIC parasail)

if (UNIX)
    find_package(ZLIB)
    target_link_libraries(libnmssearch PUBLIC ZLIB::ZLIB)
endif()

# Main executable
add_executable(nmssearch src/main.cc)
target_link_libraries(nmssearch libnmssearch)

# Unit tests
add_subdirectory("external/Catch2")

add_executable(test_nmssearch tests/test_output_writer.cc)
target_link_libraries(test_nmssearch PRIVATE Catch2::Catch2WithMain)