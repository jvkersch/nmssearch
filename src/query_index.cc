#include <filesystem>
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
#include "sequence_reader.h"
#include "util.h"

namespace fs = std::filesystem;

void display(const similarity::KNNQueue<int> *results, const std::vector<std::string> &names)
{
    std::unique_ptr<similarity::KNNQueue<int>> clone(results->Clone());
    std::cout << "Obtained " << clone->Size() << " results." << std::endl;

    while (!clone->Empty())
    {
        auto *result = clone->Pop();
        std::cout << names.at(result->id()) << std::endl;
    }
}

void CollectSequencesAndNames(FASTASequenceReader &reader,
                              const NeedlemanWunschSpace &space,
                              similarity::ObjectVector &dataset,
                              std::vector<std::string> &names)
{
    FASTASequence seq;
    size_t i = 0;
    while ((seq = reader.next()) != FASTASequenceReader::UNK)
    {
        similarity::Object *item = space.CreateObjFromStr(i++, -1, seq.sequence, nullptr).release();
        dataset.push_back(item);
        names.push_back(seq.name);
    }
}

void QueryIndexCommand::run() const
{
    NeedlemanWunschSpace nwspace;

    auto sequences = fs::path(m_params.database) / "sequences.fa";
    auto index_fname = fs::path(m_params.database) / "index.bin";

    FASTASequenceReader reader(sequences);
    similarity::ObjectVector dataset;
    std::vector<std::string> names;
    CollectSequencesAndNames(reader, nwspace, dataset, names);

    int seed = 1234;
    similarity::initLibrary(seed, LIB_LOGSTDERR, NULL);

    similarity::Index<int> *index =
        similarity::MethodFactoryRegistry<int>::Instance().CreateMethod(
            true,
            "vptree",
            "custom",
            nwspace,
            dataset);

    index->LoadIndex(index_fname);

    similarity::AnyParams queryParams({"alphaLeft=1.0", "alphaRight=1.0"});
    index->SetQueryTimeParams(queryParams);

    std::cout << "Loaded index for " << dataset.size() << " sequences" << std::endl;

    // Read query sequences
    FASTASequenceReader query_reader(m_params.query);
    similarity::ObjectVector queryset = CollectSequences(query_reader, nwspace);
    std::cout << "Loaded " << queryset.size() << " sequences." << std::endl;

    int k = 4; // TODO make configurable
    for (auto query : queryset)
    { // TODO make batch
        similarity::KNNQuery<int> knnquery(nwspace, query, k);
        index->Search(&knnquery);
        display(knnquery.Result(), names);
    }

    delete index; // TODO wrap in unique_ptr

    for (auto *item : dataset)
    {
        delete item; // TODO Make RAII
    }

    for (auto *item : queryset)
    {
        delete item; // TODO Make RAII
    }
}