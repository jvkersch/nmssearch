#ifndef _DUMP_COMMAND_H_
#define _DUMP_COMMAND_H_

#include "parameters.h"

class DumpIndexCommand
{
public:
    DumpIndexCommand(const Parameters &params) : m_params(params) {}
    void run() const;

private:
    const Parameters &m_params;
};

#endif // _DUMP_COMMAND_H_