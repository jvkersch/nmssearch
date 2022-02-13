#ifndef _OUTPUT_WRITER_H_
#define _OUTPUT_WRITER_H_

#include <iostream>

#include "nw_align.h"

class OutputWriter
{
public:
    void display(const AlignStats &r, std::ostream& output) const;
    void display(const std::vector<AlignStats>& results, std::ostream& output) const;
};

#endif // _OUTPUT_WRITER_H_