#include <iomanip>
#include <iostream>
#include <vector>

#include "sequence_reader.h"
#include "output_writer.h"

#include "align.h"

void OutputWriter::display(const AlignStats &r, std::ostream& output) const
{
    output << r.queryId << '\t'
              << r.subjectId << '\t'
              << std::fixed << std::setprecision(2) << r.pctIdentity << '\t'
              << r.alignmentLength << '\t'
              << r.mismatches << '\t'
              << r.gapOpenings << '\t'
              << r.queryStart << '\t'
              << r.queryEnd << '\t'
              << r.subjectStart << '\t'
              << r.subjectEnd << '\t'
              << r.EValue << '\t'
              << r.bitScore << std::endl;
}

void OutputWriter::display(const std::vector<AlignStats> &rs, std::ostream& output) const
{
    for (const auto &r : rs)
    {
        display(r, output);
    }
}