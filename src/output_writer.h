#ifndef _OUTPUT_WRITER_H_
#define _OUTPUT_WRITER_H_

#include "align.h"

class OutputWriter
{
public:
    void display(const AlignStats &r) const;
    void display(const std::vector<AlignStats>& results) const;
};

#endif // _OUTPUT_WRITER_H_