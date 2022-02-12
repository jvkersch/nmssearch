#include <algorithm>
#include <iostream>

// nmslib
#include "index.h"
#include "init.h"
#include "knnquery.h"
#include "knnqueue.h"
#include "methodfactory.h"
#include "object.h"
#include "params.h"

// local
#include "query_index.h"
#include "nw_space.h"
#include "output_writer.h"
#include "sequence_container.h"
#include "sequence_reader.h"

std::vector<AlignStats> PrepareQueryResults(
    const FASTASequence &query,
    const similarity::KNNQueue<int> *results,
    const SequenceContainer &sequences,
    const Aligner &aligner)
{
    std::vector<AlignStats> query_results;
    std::unique_ptr<similarity::KNNQueue<int>> clone(results->Clone());

    while (!clone->Empty())
    {
        auto *result = clone->Pop();
        auto target = sequences[result->id()];

        auto stats = aligner.align_stats(query, target);
        query_results.push_back(stats);
    }

    std::sort(std::begin(query_results),
              std::end(query_results),
              [](const AlignStats &a, const AlignStats &b)
              {
                  return a.pctIdentity > b.pctIdentity;
              });

    return query_results;
}

void QueryIndexCommand::run() const
{
    NeedlemanWunschSpace nwspace;
    Aligner aligner;

    // Read database
    FASTASequenceReader reader(m_params.database_path / "sequences.fa");
    SequenceContainer database(reader);

    // Read query sequences
    FASTASequenceReader query_reader(m_params.query_path);
    SequenceContainer queries(query_reader);

    std::cerr << "Loaded " << queries.size() << " query sequence(s)." << std::endl;

    int seed = 1234;
    similarity::initLibrary(seed, LIB_LOGNONE, NULL);

    // Create index
    similarity::AnyParams queryParams;
    std::string method_name;

    if (m_params.index_algorithm == IndexAlgorithm::hnsw)
    {
        method_name = "hnsw";
    }
    else if (m_params.index_algorithm == IndexAlgorithm::vptree)
    {
        method_name = "vptree";
        queryParams = similarity::AnyParams({"alphaLeft=1.0", "alphaRight=1.0"});
    }

    auto index = std::unique_ptr<similarity::Index<int>>(
        similarity::MethodFactoryRegistry<int>::Instance().CreateMethod(
            true,
            method_name,
            "custom",
            nwspace,
            database.getDataset()));

    index->LoadIndex(m_params.database_path / "index.bin");
    index->SetQueryTimeParams(queryParams);

    std::cerr << "Loaded index for " << database.size() << " sequences" << std::endl;
    std::cerr << "Index type " << IndexAlgorithmToString(m_params.index_algorithm) << std::endl;

    OutputWriter writer;

    for (auto query : queries.getDataset())
    { // TODO make batch
        similarity::KNNQuery<int> knnquery(nwspace, query, m_params.k);
        index->Search(&knnquery);

        auto query_results = PrepareQueryResults(queries[query->id()], knnquery.Result(), database, aligner);
        writer.display(query_results, std::cout);
    }
}