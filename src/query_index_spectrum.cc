#include <algorithm>
#include <iostream>

// nmslib
#include "factory/init_methods.h"
#include "index.h"
#include "init.h"
#include "knnquery.h"
#include "knnqueue.h"
#include "methodfactory.h"
#include "object.h"
#include "params.h"

// local
#include "alignment_space.h"
#include "kmer_space.h"
#include "nw_align.h"
#include "output_writer.h"
#include "query_index_spectrum.h"
#include "sequence_container.h"
#include "sequence_reader.h"
#include "spectrum_io.h"
#include "spectrum_loader.h"

std::vector<AlignStats>
PrepareQueryResults(const Spectrum &query,
                    const similarity::KNNQueue<float> *results,
                    const std::vector<Spectrum> &spectra) {
  std::vector<AlignStats> query_results;
  std::unique_ptr<similarity::KNNQueue<float>> clone(results->Clone());

  while (!clone->Empty()) {
    auto *result = clone->Pop();
    auto target = spectra.at(result->id());

    AlignStats stats;
    stats.queryId = query.name();

    auto subjectId = spectra.at(result->id()).name();
    stats.subjectId = subjectId;
    stats.pctIdentity = 1 - cosine_distance(query, target); // will do for now

    query_results.push_back(stats);
  }

  std::sort(std::begin(query_results), std::end(query_results),
            [](const AlignStats &a, const AlignStats &b) {
              return a.pctIdentity > b.pctIdentity;
            });

  return query_results;
}


inline similarity::ObjectVector CreateDataset(size_t n, size_t offset = 0) {
  similarity::ObjectVector indices;
  for (size_t i = offset; i < offset + n; i++) {
    indices.push_back(new similarity::Object(i, -1, 0, nullptr));
  }
  return indices;
}

void SpectrumQueryIndexCommand::run() const {

  // Read database
  auto path = m_params.database_path;
  if (m_params.index_algorithm != IndexAlgorithm::bruteforce) {
    path /= "sequences.fa";
  }

  auto spectra = deserialize(m_params.database_path / "spectra.bin");
  auto indices = CreateDataset(spectra.size());

  // Read query sequences
  auto query_spectra =
      LoadSpectraFromFile(m_params.query_path, m_params.kmer_length);
  auto query_indices = CreateDataset(query_spectra.size(), spectra.size());

  std::vector<Spectrum> combined;
  combined.reserve(spectra.size() + query_spectra.size());
  combined.insert(combined.end(), spectra.begin(), spectra.end());
  combined.insert(combined.end(), query_spectra.begin(), query_spectra.end());
  KMerSpace space(&combined);

  std::cerr << "Loaded " << query_spectra.size() << " query sequence(s)."
            << std::endl;

  int seed = 1234;
  similarity::initLibrary(seed, LIB_LOGNONE, NULL);

  // Create index
  similarity::AnyParams queryParams;
  std::string method_name;

  if (m_params.index_algorithm == IndexAlgorithm::hnsw) {
    method_name = METH_HNSW;
  } else if (m_params.index_algorithm == IndexAlgorithm::vptree) {
    method_name = METH_VPTREE;
    queryParams = similarity::AnyParams({"alphaLeft=1.0", "alphaRight=1.0"});
  } else if (m_params.index_algorithm == IndexAlgorithm::bruteforce) {
    method_name = METH_SEQ_SEARCH;
  }

  auto index = std::unique_ptr<similarity::Index<float>>(
      similarity::MethodFactoryRegistry<float>::Instance().CreateMethod(
          true, method_name, "custom", space, indices));

  if (m_params.index_algorithm == IndexAlgorithm::bruteforce) {
    index->CreateIndex(queryParams);
  } else {
    index->LoadIndex(m_params.database_path / "index.bin");
  }

  index->SetQueryTimeParams(queryParams);

  std::cerr << "Loaded index for " << spectra.size() << " sequences"
            << std::endl;
  std::cerr << "Index type " << IndexAlgorithmToString(m_params.index_algorithm)
            << std::endl;

  for (auto query : query_indices) { // TODO make batch
    similarity::KNNQuery<float> knnquery(space, query, m_params.k);
    index->Search(&knnquery);

    auto query_results =
        PrepareQueryResults(query_spectra.at(query->id() - spectra.size()),
                            knnquery.Result(), spectra);
    m_writer.display(query_results, std::cout);
  }
}
