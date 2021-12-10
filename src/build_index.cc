#include <iostream>

// nmslib
#include "index.h"
#include "init.h"
#include "methodfactory.h"
#include "object.h"
#include "params.h"

// local
#include "build_index.h"
#include "nw_space.h"
#include "sequence_reader.h"


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

void BuildIndexCommand::run() const
{
    NeedlemanWunschSpace nwspace;

    std::cout << "Building index for " << m_params.sequences << std::endl;
    FASTASequenceReader reader(m_params.sequences);
    similarity::ObjectVector dataset = CollectSequences(reader, nwspace);
    std::cout << "Read " << dataset.size() << " sequences." << std::endl;

    int seed = 1234;
    similarity::initLibrary(seed, LIB_LOGSTDERR, NULL);

    similarity::Index<int> *index =
        similarity::MethodFactoryRegistry<int>::Instance().CreateMethod(
            true,
            "vptree",
            "custom",
            nwspace,
            dataset);

    similarity::AnyParams indexParams({"bucketSize=1", "selectPivotAttempts=1"});
    similarity::AnyParams queryParams({"alphaLeft=1.0", "alphaRight=1.0"});

    index->CreateIndex(indexParams);
    index->SaveIndex("my_vptree.bin");

    std::cout << "Saved index to my_vptree.bin" << std::endl; // TODO make output name a parameter.

    for (auto *item : dataset)
    {
        delete item; // TODO Make RAII
    }
}
