#ifndef _TESTING_HELPERS_H_
#define _TESTING_HELPERS_H_

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>


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

#endif // _TESTING_HELPERS_H_