#ifndef _ALIGN_H_
#define _ALIGN_H_

class FASTASequence;
class AlignStats;

#include "alignment_stats.h"

class NeedlemanWunschAligner
{
public:
    NeedlemanWunschAligner(int gap_open=12, int gap_extend=4):
        m_gap_open(gap_open), m_gap_extend(gap_extend) {}

    // FIXME: API is inconsistent: const char* <-> FASTASequence...
    int align(const char *s1, size_t len1, const char *s2, size_t len2) const;
    AlignStats align_stats(const FASTASequence &s1, const FASTASequence &s2) const;

    int ncalls() const { return m_ncalls; }

private:
    int m_gap_open;
    int m_gap_extend;

    // This needs to be mutable so we can keep align const.
    mutable int m_ncalls = 0;
};

#endif // _ALIGN_H_