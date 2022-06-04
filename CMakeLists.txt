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
    src/command_parser.cc 
    src/build_index.cc 
    src/dump_index.cc
    src/kmer_space.cc
    src/nw_align.cc 
    src/sequence_reader.cc 
    src/query_index.cc 
    src/sequence_container.cc
    src/spectrum.cc 
    src/spectrum_io.cc
    src/output_writer.cc)
set_target_properties(libnmssearch PROPERTIES OUTPUT_NAME "nmssearch")

# For NMSLIB we have to set the include path manually, since it is not specified in the library CMakeLists.txt
target_link_libraries(libnmssearch PUBLIC NonMetricSpaceLib)
target_include_directories(libnmssearch PRIVATE "${PROJECT_SOURCE_DIR}/external/nmslib/similarity_search/include")

target_include_directories(libnmssearch PRIVATE "${PROJECT_SOURCE_DIR}/external/cli11")

target_include_directories(libnmssearch PRIVATE "${PROJECT_SOURCE_DIR}/external/cereal/include")

target_link_libraries(libnmssearch PUBLIC parasail)

find_package(Threads REQUIRED)
target_link_libraries(libnmssearch PRIVATE Threads::Threads)

if (UNIX)
    find_package(ZLIB)
    target_link_libraries(libnmssearch PUBLIC ZLIB::ZLIB)
endif()

# Main executable
add_executable(nmssearch src/main.cc)
target_link_libraries(nmssearch PRIVATE libnmssearch)

# Unit tests
add_subdirectory("external/Catch2")

add_executable(test_nmssearch 
   tests/test_kmer_space.cc
   tests/test_output_writer.cc
   tests/test_parameter_handling.cc
   tests/test_spectrum.cc
   tests/test_spectrum_io.cc
   tests/test_spectrum_loader.cc)
target_include_directories(test_nmssearch PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_include_directories(test_nmssearch PRIVATE "${PROJECT_SOURCE_DIR}/external/cereal/include")
target_include_directories(test_nmssearch PRIVATE "${PROJECT_SOURCE_DIR}/external/nmslib/similarity_search/include")
target_link_libraries(test_nmssearch PRIVATE libnmssearch)
target_link_libraries(test_nmssearch PRIVATE Catch2::Catch2WithMain)