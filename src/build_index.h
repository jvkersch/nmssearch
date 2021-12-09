#ifndef _BUILD_COMMAND_H_
#define _BUILD_COMMAND_H_

#include "parameters.h"

class BuildIndexCommand
{
public:
    BuildIndexCommand(const Parameters &params) : _params(params) {}
    void run() const;

private:
    const Parameters &_params;
};

#endif // _BUILD_COMMAND_H_