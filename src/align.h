#ifndef _ALIGN_H_
#define _ALIGN_H_

#include <cstddef>

class Aligner
{
public:
    Aligner();

    int align(const char *s1, size_t len1, const char *s2, size_t len2) const;

private:
    // TODO: Make alignment matrix and gap penalties configurable
};

#endif // _ALIGN_H_