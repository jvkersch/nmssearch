#ifndef _OUTPUT_WRITER_H_
#define _OUTPUT_WRITER_H_

#include "sequence_container.h"

class FASTASequence;

class OutputWriter
{
public:
    OutputWriter(const SequenceContainer &sequences);

    void display(const FASTASequence& query, size_t targetId) const;

private:
    const SequenceContainer &m_sequences;
};

#endif // _OUTPUT_WRITER_H_