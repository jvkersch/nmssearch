#ifndef _ALIGN_STATS_H_
#define _ALIGN_STATS_H_

#include <cstddef>
#include <string>

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

#endif // _ALIGN_STATS_H_