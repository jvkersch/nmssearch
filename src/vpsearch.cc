#include <string>

#include "init.h"
#include "index.h"
#include "params.h"
#include "rangequery.h"
#include "knnquery.h"
#include "knnqueue.h"
#include "methodfactory.h"
#include "space/space_string.h"

#include <parasail.h>
#include <parasail/io.h>
#include <parasail/matrices/nuc44.h>

struct Seq {
    Seq(const std::string& label, const std::string& seq): label(label), seq(seq) {}

    std::string label;
    std::string seq;
};

int nw(const char* p1, size_t len1, const char* p2, size_t len2) {
    parasail_result_t* result = parasail_nw_scan_16(p1, len1, p2, len2, 12, 4, &parasail_nuc44);
    int score = parasail_result_get_score(result);
    parasail_result_free(result);

    return 5*len1 + 5*len2 - 2*score;
}

class SpaceNW : public similarity::StringSpace<int> {
 public:
  explicit SpaceNW() {}
  virtual ~SpaceNW() {}
  virtual std::string StrDesc() const { return "Needleman-Wunsch distance"; }

 protected:
  virtual int HiddenDistance(const similarity::Object* obj1, const similarity::Object* obj2) const {
    CHECK(obj1->datalength() > 0);
    CHECK(obj2->datalength() > 0);
    const char* x = reinterpret_cast<const char*>(obj1->data());
    const char* y = reinterpret_cast<const char*>(obj2->data());
    const size_t len1 = obj1->datalength() / sizeof(char);
    const size_t len2 = obj2->datalength() / sizeof(char);

    return nw(x, len1, y, len2);
  }
  DISABLE_COPY_AND_ASSIGN(SpaceNW);
};

similarity::ObjectVector readSequenceData (const std::string& fname, const SpaceNW& nwSpace) {
    similarity::ObjectVector data;
    parasail_sequences_t *sequences = parasail_sequences_from_file(fname.c_str());
    for (size_t i = 0; i < sequences->l; i++) {
        parasail_sequence_t seq = sequences->seqs[i];
        std::string sdata(seq.seq.s, seq.seq.l);
        similarity::Object* item = nwSpace.CreateObjFromStr(i, -1, sdata, nullptr).release();
        data.push_back(item);
    }
    parasail_sequences_free(sequences);
    return data;
}

void display(const similarity::KNNQueue<int>* results) {
    std::unique_ptr<similarity::KNNQueue<int>> clone(results->Clone());
    while (!clone->Empty()) {
        auto* result = clone->Pop();
        std::cout << result->id() << std::endl;
    }
}

int main() {
    SpaceNW nwspace;
    
    similarity::ObjectVector dataset = readSequenceData("silva-1000.fa", nwspace);
    similarity::ObjectVector queryset = readSequenceData("mothur-sop-asv_1.fasta", nwspace);
    std::cout << "Read "<< dataset.size() << " sequences." << std::endl;

    int seed = 1234;
    similarity::initLibrary(seed, LIB_LOGSTDERR, NULL);

    similarity::Index<int>* index = 
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

    std:cerr << "done saveing" << std::endl;

    similarity::Index<int>* other = 
        similarity::MethodFactoryRegistry<int>::Instance().CreateMethod(
            true,
            "vptree",
            "custom",
            nwspace,
            dataset);

    other->LoadIndex("my_vptree.bin");


    index->SetQueryTimeParams(queryParams);
    other->SetQueryTimeParams(queryParams);

    std::cout << "Searching for 4 neighbors..." << std::endl;
    int k = 4;
    for (auto query: queryset) {
        similarity::KNNQuery<int> knnquery(nwspace, query, k);
        other->Search(&knnquery);
        display(knnquery.Result());
    }

    std::cerr << "Original index" << std::endl;
    for (auto query: queryset) {
        similarity::KNNQuery<int> knnquery(nwspace, query, k);
        index->Search(&knnquery);
        display(knnquery.Result());
    }

    delete index;
    delete other;
    for (auto* item : dataset) {
        delete item;
    }
    for (auto* item : queryset) {
        delete item;
    }

    return 0;
}