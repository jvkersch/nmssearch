#ifndef _SEQUENCE_READER_H_
#define _SEQUENCE_READER_H_

#include <string>

#include "parasail/io.h"

class FASTASequence
{
public:
    std::string name;
    std::string sequence;

    FASTASequence() {}
    FASTASequence(std::string name, std::string sequence) : sequence(sequence), name(name) {}

    bool operator==(const FASTASequence &other) const
    {
        return ((name == other.name) && (sequence == other.sequence));
    }
    bool operator!=(const FASTASequence &other) const
    {
        return !(*this == other);
    }
};

class FASTASequenceReader
{
public:
    FASTASequenceReader(const std::string &filename);
    ~FASTASequenceReader();

    FASTASequence next();

    static const FASTASequence UNK;

private:
    parasail_sequences_t *m_sequences;
    size_t m_i;
};

#endif // _SEQUENCE_READER_H_