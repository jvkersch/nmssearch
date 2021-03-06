#ifndef _SEQUENCE_CONTAINER_H_
#define _SEQUENCE_CONTAINER_H_

#include <vector>

// nmslib
#include "object.h"

// local
#include "sequence_reader.h"

class SequenceContainer
{
public:
    SequenceContainer(FASTASequenceReader &reader);
    ~SequenceContainer();

    FASTASequence operator[](size_t i) const
    {
        auto obj = m_sequences[i];

        std::string seq(obj->data(), obj->datalength() / sizeof(char));
        return FASTASequence(m_names[i], seq);
    }

    const similarity::ObjectVector &getDataset() const
    {
        return m_sequences;
    }

    size_t size() const
    {
        return m_sequences.size();
    }

private:
    similarity::ObjectVector m_sequences;
    std::vector<std::string> m_names;
};

#endif // _SEQUENCE_CONTAINER_H_