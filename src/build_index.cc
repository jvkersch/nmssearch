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

    std::cerr << "Building index for " << m_params.sequences_path << " using algorithm "
              << IndexAlgorithmToString(m_params.index_algorithm) << std::endl;

    // Read sequences
    FASTASequenceReader reader(m_params.sequences_path);
    SequenceContainer database(reader);

    std::cerr << "Read " << database.size() << " sequences." << std::endl;

    int seed = 1234;
    similarity::initLibrary(seed, LIB_LOGNONE, NULL);

    // Create index
    std::unique_ptr<similarity::Index<int>> index;
    similarity::AnyParams indexParams;
    fs::path filename;  // TODO make configurable

    if (m_params.index_algorithm == IndexAlgorithm::hnsw)
    {
        index.reset(
            similarity::MethodFactoryRegistry<int>::Instance().CreateMethod(
                true,
                "hnsw",
                "custom",
                nwspace,
                database.getDataset()));
        
        filename = "hnsw";
    }
    else if (m_params.index_algorithm == IndexAlgorithm::vptree)
    {
        index.reset(
            similarity::MethodFactoryRegistry<int>::Instance().CreateMethod(
                true,
                "vptree",
                "custom",
                nwspace,
                database.getDataset()));

        indexParams = similarity::AnyParams({"bucketSize=1", "selectPivotAttempts=1"});
        filename = "vptree";
    }

    index->CreateIndex(indexParams);

    // Persist the index
    fs::remove_all(filename);
    fs::create_directories(filename);
    fs::copy(m_params.sequences_path, filename/"sequences.fa");
    index->SaveIndex(filename / "index.bin");

    std::cerr << "Saved index to " << filename << std::endl; // TODO make output name a parameter.
}
