#ifndef _ALIGN_QUERY_INDEX_H_
#define _ALIGN_QUERY_INDEX_H_

#include "parameters.h"
#include "query_index.h"


class AlignQueryIndexCommand: public QueryIndexCommand
{
public:
    AlignQueryIndexCommand(const Parameters& params) : QueryIndexCommand(params) {}

    void run() const override;
};

#endif // _ALIGN_QUERY_INDEX_H_