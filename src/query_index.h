#ifndef _QUERY_INDEX_H_
#define _QUERY_INDEX_H_

// local
#include "parameters.h"
#include "output_writer.h"

// nmslib
#include "params.h"

class QueryIndexCommand {
public:
    QueryIndexCommand(const Parameters &params, const BaseOutputWriter &writer) :
        m_params(params), m_writer(writer) {}

    virtual void run() const = 0;

protected:
    const Parameters &m_params;
    const BaseOutputWriter &m_writer;

    virtual similarity::AnyParams getQueryParameters() const;
    
};

#endif // _QUERY_INDEX_H_