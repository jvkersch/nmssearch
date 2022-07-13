#ifndef _TESTING_HELPERS_H_
#define _TESTING_HELPERS_H_

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>

#include <catch2/catch_test_macros.hpp>


namespace fs = std::filesystem;


// Create a temporary file with the given contents. Caller needs to unlink the file.
inline std::string CreateTempFile(const std::string &contents) {
    char temp_filename[] = "/tmp/nms_temp_file.XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        std::cerr << "Could not create temporary file" << std::endl;
        exit(1);
    }
    FILE* f = fdopen(fd, "w");

    std::fwrite(contents.c_str(), 1, contents.size(), f);
    std::fclose(f);

    return temp_filename;
}

// Unlink the given temporary file.
inline void RemoveTempFile(const std::string &filename) {
    std::remove(filename.c_str());
}

inline fs::path testing_artifact(std::string name) {
    fs::path full_path = fs::path(TEST_ARTIFACT_DIRECTORY) / name;
    if (!fs::exists(full_path)) {
        std::stringstream ss;
        ss << "File not found: " << full_path;
        throw std::runtime_error(ss.str());
    }
    return full_path;
}

inline void require_exists(fs::path p) {
    REQUIRE(fs::exists(p));
}

#endif // _TESTING_HELPERS_H_