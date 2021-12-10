#include "sequence_reader.h"

const FASTASequence FASTASequenceReader::UNK{"unk", "unk"};

FASTASequenceReader::FASTASequenceReader(const std::string &filename) : m_sequences(nullptr), m_i(0)
{
    m_sequences = parasail_sequences_from_file(filename.c_str());
}

FASTASequenceReader::~FASTASequenceReader()
{
    if (m_sequences)
    {
        parasail_sequences_free(m_sequences);
        m_sequences = nullptr;
    }
}

FASTASequence FASTASequenceReader::next()
{
    if (m_i == m_sequences->l)
    {
        return UNK;
    }

    parasail_sequence_t seq = m_sequences->seqs[m_i++];
    std::string sequence(seq.seq.s, seq.seq.l);
    std::string name(seq.name.s, seq.name.l);

    return FASTASequence(name, sequence);
}