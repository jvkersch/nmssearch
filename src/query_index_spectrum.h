#ifndef _SPECTRUM_QUERY_INDEX_H_
#define _SPECTRUM_QUERY_INDEX_H_

#include "output_writer.h"
#include "parameters.h"
#include "query_index.h"


class SpectrumQueryIndexCommand: public QueryIndexCommand
{
public:
    SpectrumQueryIndexCommand(const Parameters& params, const BaseOutputWriter& writer) : 
        QueryIndexCommand(params, writer) {}

    void run() const override;
};

#endif // _SPECTRUM_QUERY_INDEX_H_