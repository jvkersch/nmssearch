#ifndef _OUTPUT_WRITER_H_
#define _OUTPUT_WRITER_H_

#include <iostream>
#include <vector>

#include "alignment_stats.h"

class BaseOutputWriter
{
public:
    virtual void display(const AlignStats &r, std::ostream& output) const = 0;
    virtual void display(const std::vector<AlignStats>& results, std::ostream& output) const = 0;
};

class OutputWriter: public BaseOutputWriter
{
public:
    void display(const AlignStats &r, std::ostream& output) const override;
    void display(const std::vector<AlignStats>& results, std::ostream& output) const override;
};

#endif // _OUTPUT_WRITER_H_