#ifndef _ALIGN_H_
#define _ALIGN_H_

class FASTASequence;
class AlignStats;

#include "alignment_stats.h"

class NeedlemanWunschAligner
{
public:
    NeedlemanWunschAligner();

    int align(const char *s1, size_t len1, const char *s2, size_t len2) const;
    AlignStats align_stats(const FASTASequence &s1, const FASTASequence &s2) const;

private:
    // TODO: Make alignment matrix and gap penalties configurable
};

#endif // _ALIGN_H_