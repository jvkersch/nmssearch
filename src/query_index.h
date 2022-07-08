#ifndef _QUERY_INDEX_H_
#define _QUERY_INDEX_H_

#include "parameters.h"
#include "output_writer.h"


class QueryIndexCommand {
public:
    QueryIndexCommand(const Parameters &params) : m_params(params) {}
    QueryIndexCommand(const Parameters &params, const OutputWriter writer) :
        m_params(params), m_writer(writer) {}

    virtual void run() const = 0;

protected:
    const Parameters &m_params;
    const OutputWriter m_writer;
};

#endif // _QUERY_INDEX_H_