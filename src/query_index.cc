#include "query_index.h"


similarity::AnyParams QueryIndexCommand::getQueryParameters() const {

    similarity::AnyParams queryParams;

    if (m_params.index_algorithm == IndexAlgorithm::vptree) {
        queryParams.AddChangeParam("alphaLeft", 1.0);
        queryParams.AddChangeParam("alphaRight", 1.0);
    }
    
    return queryParams;
}