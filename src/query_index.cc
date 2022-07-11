#include "query_index.h"


similarity::AnyParams QueryIndexCommand::getQueryParameters() const {

    similarity::AnyParams queryParams;

    auto algo = m_params.index_algorithm;
    if (algo == IndexAlgorithm::vptree) {
        queryParams.AddChangeParam("alphaLeft", 1.0);
        queryParams.AddChangeParam("alphaRight", 1.0);
    } else if ((algo == IndexAlgorithm::bruteforce) || (algo == IndexAlgorithm::bruteforce_kmer)) {
        queryParams.AddChangeParam("threadQty", m_params.indexThreadQty);
    }
    
    return queryParams;
}