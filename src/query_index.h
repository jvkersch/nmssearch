#ifndef _QUERY_INDEX_H_
#define _QUERY_INDEX_H_

#include "parameters.h"


class QueryIndexCommand {
public:
    QueryIndexCommand(const Parameters &params) : m_params(params) {}

    virtual void run() const = 0;

protected:
    const Parameters &m_params;
};

#endif // _QUERY_INDEX_H_