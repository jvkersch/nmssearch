#include <filesystem>
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
#include "util.h"

namespace fs = std::filesystem;

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

    fs::remove_all("my_vptree");
    fs::create_directories("my_vptree");
    fs::copy(m_params.sequences, "my_vptree/sequences.fa");
    index->SaveIndex("my_vptree/index.bin");

    std::cout << "Saved index to my_vptree.bin" << std::endl; // TODO make output name a parameter.

    delete index; // TODO wrap in unique_ptr

    for (auto *item : dataset)
    {
        delete item; // TODO Make RAII
    }
}
