#include "util.h"

similarity::ObjectVector CollectSequences(FASTASequenceReader &reader, const NeedlemanWunschSpace &space)
{
    similarity::ObjectVector data;

    FASTASequence seq;
    size_t i = 0;
    while ((seq = reader.next()) != FASTASequenceReader::UNK)
    {
        similarity::Object *item = space.CreateObjFromStr(i++, -1, seq.sequence, nullptr).release();
        data.push_back(item);
    }
    return data;
}