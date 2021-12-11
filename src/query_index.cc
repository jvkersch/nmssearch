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
#include "sequence_container.h"
#include "sequence_reader.h"
#include "util.h"


void display(const similarity::KNNQueue<int> *results, const SequenceContainer &sequences)
{
    std::unique_ptr<similarity::KNNQueue<int>> clone(results->Clone());
    std::cout << "Obtained " << clone->Size() << " results." << std::endl;

    while (!clone->Empty())
    {
        auto *result = clone->Pop();
        std::cout << sequences[result->id()].name << std::endl;
    }
}

void QueryIndexCommand::run() const
{
    NeedlemanWunschSpace nwspace;

    // Read database
    FASTASequenceReader reader(m_params.database_path / "sequences.fa");
    SequenceContainer database(reader);

    // Read query sequences
    FASTASequenceReader query_reader(m_params.query_path);
    SequenceContainer queries(query_reader);

    std::cout << "Loaded " << queries.size() << " sequences." << std::endl;

    int seed = 1234;
    similarity::initLibrary(seed, LIB_LOGSTDERR, NULL);

    auto index = std::unique_ptr<similarity::Index<int>>(
        similarity::MethodFactoryRegistry<int>::Instance().CreateMethod(
            true,
            "vptree",
            "custom",
            nwspace,
            database.getDataset()));

    index->LoadIndex(m_params.database_path / "index.bin");

    similarity::AnyParams queryParams({"alphaLeft=1.0", "alphaRight=1.0"});
    index->SetQueryTimeParams(queryParams);

    std::cout << "Loaded index for " << database.size() << " sequences" << std::endl;

    int k = 4; // TODO make configurable
    for (auto query : queries.getDataset())
    { // TODO make batch
        similarity::KNNQuery<int> knnquery(nwspace, query, k);
        index->Search(&knnquery);
        display(knnquery.Result(), database);
    }
}