#ifndef _ALIGN_QUERY_INDEX_H_
#define _ALIGN_QUERY_INDEX_H_

#include "parameters.h"

class AlignQueryIndexCommand
{
public:
    AlignQueryIndexCommand(const Parameters &params) : m_params(params) {}
    void run() const;

private:
    const Parameters &m_params;
};

#endif // _ALIGN_QUERY_INDEX_H_