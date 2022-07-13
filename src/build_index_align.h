#ifndef _BUILD_INDEX_ALIGN_H_
#define _BUILD_INDEX_ALIGN_H_

#include "build_index.h"
#include "parameters.h"

class AlignBuildIndexCommand: public BuildIndexCommand
{
public:
    AlignBuildIndexCommand(const Parameters &params) : BuildIndexCommand(params) {}
    void run() const override;
};

#endif // _BUILD_INDEX_ALIGN_H_