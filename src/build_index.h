#ifndef _BUILD_INDEX_H_
#define _BUILD_INDEX_H_

#include "parameters.h"

class BuildIndexCommand
{
public:
    BuildIndexCommand(const Parameters &params) : m_params(params) {}
    virtual void run() const = 0;

protected:
    const Parameters &m_params;
};

#endif // _BUILD_INDEX_H_