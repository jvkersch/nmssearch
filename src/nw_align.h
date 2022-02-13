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

    int ncalls() const { return m_ncalls; }

private:
    // TODO: Make alignment matrix and gap penalties configurable

    // This needs to be mutable so we can keep align const.
    mutable int m_ncalls = 0;
};

#endif // _ALIGN_H_