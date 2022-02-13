#ifndef _ALIGN_H_
#define _ALIGN_H_

#include <cstddef>
#include <string>

class FASTASequence;
class AlignStats;

class Aligner
{
public:
    Aligner();

    int align(const char *s1, size_t len1, const char *s2, size_t len2) const;
    AlignStats align_stats(const FASTASequence &s1, const FASTASequence &s2) const;

private:
    // TODO: Make alignment matrix and gap penalties configurable
};

class AlignStats
{
public:
    std::string queryId;
    std::string subjectId;
    double pctIdentity;
    int alignmentLength;
    int mismatches;
    int gapOpenings;
    size_t queryStart;
    size_t queryEnd;
    size_t subjectStart;
    size_t subjectEnd;
    double EValue;
    int bitScore;
};

#endif // _ALIGN_H_