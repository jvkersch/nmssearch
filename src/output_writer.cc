#include <iostream>

#include "sequence_reader.h"
#include "output_writer.h"

#include "align.h"

OutputWriter::OutputWriter(const SequenceContainer &sequences) : m_sequences(sequences) {}

void OutputWriter::display(const FASTASequence &query, size_t targetId) const
{
    auto target = m_sequences[targetId];

    Aligner aligner;
    double dist = aligner.align_stats(
        query.sequence.c_str(),
        query.sequence.size(),
        target.sequence.c_str(),
        target.sequence.size());

    std::cout << "query: " << query.name << ", target: " << target.name << ", pct similarity:" << dist << std::endl;
}
