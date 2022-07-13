#ifndef _BUILD_INDEX_SPECTRUM_H_
#define _BUILD_INDEX_SPECTRUM_H_

#include "build_index.h"
#include "parameters.h"

class SpectrumBuildIndexCommand: public BuildIndexCommand
{
public:
    SpectrumBuildIndexCommand(const Parameters &params) : BuildIndexCommand(params) {}
    void run() const override;
};

#endif // _BUILD_INDEX_SPECTRUM_H_