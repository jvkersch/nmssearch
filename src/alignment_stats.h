#ifndef _ALIGN_STATS_H_
#define _ALIGN_STATS_H_

#include <cstddef>
#include <string>

class AlignStats
{
public:
    std::string queryId = "";
    std::string subjectId = "";
    double pctIdentity = 0.0;
    int alignmentLength = 0;
    int mismatches = 0;
    int gapOpenings = 0;
    size_t queryStart = 0;
    size_t queryEnd = 0;
    size_t subjectStart = 0;
    size_t subjectEnd = 0;
    double EValue = 0.0;
    int bitScore = 0;
};

#endif // _ALIGN_STATS_H_