#include <filesystem>
#include <iostream>

// nmslib
#include "index.h"
#include "init.h"
#include "methodfactory.h"
#include "object.h"
#include "params.h"

// local
#include "alignment_space.h"
#include "build_index_spectrum.h"
#include "nw_align.h"
#include "sequence_container.h"
#include "sequence_reader.h"

#include "kmer_space.h"
#include "spectrum.h"
#include "spectrum_io.h"
#include "spectrum_loader.h"

inline similarity::ObjectVector CreateDataset(size_t n) {
  similarity::ObjectVector indices;
  for (size_t i = 0; i < n; i++) {
    indices.push_back(new similarity::Object(i, -1, 0, nullptr));
  }
  return indices;
}

namespace fs = std::filesystem;

void SpectrumBuildIndexCommand::run() const {

  std::cerr << "Building index for " << m_params.sequences_path
            << " using algorithm "
            << IndexAlgorithmToString(m_params.index_algorithm) << std::endl;

  auto database =
      LoadSpectraFromFile(m_params.sequences_path, m_params.kmer_length);
  auto indices = CreateDataset(database.size());

  KMerSpace space(&database);

  std::cerr << "Read " << database.size() << " sequences." << std::endl;

  int seed = 1234;
  similarity::initLibrary(seed, LIB_LOGNONE, NULL);

  // Persist spectra
  auto db = m_params.database_path;
  fs::remove_all(db);
  fs::create_directories(db);
  fs::copy(m_params.sequences_path, db / "sequences.fa");
  serialize(database, db / "spectra.bin");

 // Create index
  similarity::AnyParams indexParams;
  std::string method_name;
  indexParams.AddChangeParam("indexThreadQty", m_params.indexThreadQty);

  if (m_params.index_algorithm == IndexAlgorithm::hnsw_kmer) {
    method_name = "hnsw";
    indexParams.AddChangeParam("M", m_params.M);
    indexParams.AddChangeParam("efConstruction", m_params.efConstruction);
  
    std::cerr << "Parameters: " << indexParams.ToString() << std::endl;

    auto index = std::unique_ptr<similarity::Index<float>>(
        similarity::MethodFactoryRegistry<float>::Instance().CreateMethod(
            true, method_name, "custom", space, indices));

    index->CreateIndex(indexParams);
    std::cout << "INDEX SIZE: " << index->GetSize() << std::endl;

    index->SaveIndex(db / "index.bin");
  }

  std::cerr << "Saved index to " << db << std::endl;
}
