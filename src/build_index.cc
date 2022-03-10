#include <filesystem>
#include <iostream>

// nmslib
#include "index.h"
#include "init.h"
#include "methodfactory.h"
#include "object.h"
#include "params.h"

// local
#include "nw_align.h"
#include "build_index.h"
#include "alignment_space.h"
#include "sequence_container.h"
#include "sequence_reader.h"

namespace fs = std::filesystem;

void BuildIndexCommand::run() const
{
    NeedlemanWunschAligner aligner;
    AlignmentSpace<NeedlemanWunschAligner> nwspace(aligner);

    std::cerr << "Building index for " << m_params.sequences_path << " using algorithm "
              << IndexAlgorithmToString(m_params.index_algorithm) << std::endl;

    // Early exit for brute-force, which doesn't build an index
    if (m_params.index_algorithm == IndexAlgorithm::bruteforce) {
        std::cerr << "Brute-force algorithm does not require an index.";
        return;
    }

    // Read sequences
    FASTASequenceReader reader(m_params.sequences_path);
    SequenceContainer database(reader);

    std::cerr << "Read " << database.size() << " sequences." << std::endl;

    int seed = 1234;
    similarity::initLibrary(seed, LIB_LOGNONE, NULL);

    // Create index
    similarity::AnyParams indexParams;
    std::string method_name;

    if (m_params.index_algorithm == IndexAlgorithm::hnsw)
    {
        method_name = "hnsw";
        indexParams.AddChangeParam("M", m_params.M);
        indexParams.AddChangeParam("efConstruction", m_params.efConstruction);
    }
    else if (m_params.index_algorithm == IndexAlgorithm::vptree)
    {
        method_name = "vptree";
        indexParams = similarity::AnyParams({"bucketSize=1", "selectPivotAttempts=1"});
    }
    std::cerr << "Parameters: " << indexParams.ToString() << std::endl;

    auto index = std::unique_ptr<similarity::Index<int>>(
        similarity::MethodFactoryRegistry<int>::Instance().CreateMethod(
            true,
            method_name,
            "custom",
            nwspace,
            database.getDataset()));

    index->CreateIndex(indexParams);

    // Persist the index
    auto db = m_params.database_path;
    fs::remove_all(db);
    fs::create_directories(db);
    fs::copy(m_params.sequences_path, db / "sequences.fa");
    index->SaveIndex(db / "index.bin");

    std::cerr << "Saved index to " << db << std::endl;

    if (m_params.instrumentation) {
        std::cerr << "Calls to aligner: " << aligner.ncalls() << std::endl;
    }
}
