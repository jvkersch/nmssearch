#include "sequence_container.h"

void CollectSequencesAndNames_(FASTASequenceReader &reader,
                               similarity::ObjectVector &dataset,
                               std::vector<std::string> &names)
{
    FASTASequence seq;
    size_t i = 0;
    while ((seq = reader.next()) != FASTASequenceReader::UNK)
    {
        auto s = seq.sequence;
        // FIXME: This is a bit dodgy: s goes out of scope, but s.c_str() happens to point
        // to the same memory as the underlying data from Parasail, so things happen to work.
        similarity::Object *item = new similarity::Object(i++, -1, s.size(), s.c_str());
        dataset.push_back(item);
        names.push_back(seq.name);
    }
}

SequenceContainer::SequenceContainer(FASTASequenceReader &reader)
{
    CollectSequencesAndNames_(reader, m_sequences, m_names);
}

SequenceContainer::~SequenceContainer()
{
    for (auto *sequence : m_sequences)
    {
        delete sequence;
    }
}