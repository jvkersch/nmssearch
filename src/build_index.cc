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
#include "sequence_container.h"
#include "sequence_reader.h"
#include "util.h"

namespace fs = std::filesystem;

void BuildIndexCommand::run() const
{
    NeedlemanWunschSpace nwspace;

    std::cerr << "Building index for " << m_params.sequences_path << std::endl;
    FASTASequenceReader reader(m_params.sequences_path);
    SequenceContainer database(reader);

    std::cerr << "Read " << database.size() << " sequences." << std::endl;

    int seed = 1234;
    similarity::initLibrary(seed, LIB_LOGNONE, NULL);

    auto index = std::unique_ptr<similarity::Index<int>>(
        similarity::MethodFactoryRegistry<int>::Instance().CreateMethod(
            true,
            "vptree",
            "custom",
            nwspace,
            database.getDataset()));

    similarity::AnyParams indexParams({"bucketSize=1", "selectPivotAttempts=1"});
    similarity::AnyParams queryParams({"alphaLeft=1.0", "alphaRight=1.0"});

    index->CreateIndex(indexParams);

    fs::remove_all("my_vptree");
    fs::create_directories("my_vptree");
    fs::copy(m_params.sequences_path, "my_vptree/sequences.fa");
    index->SaveIndex("my_vptree/index.bin");

    std::cerr << "Saved index to my_vptree.bin" << std::endl; // TODO make output name a parameter.
}
