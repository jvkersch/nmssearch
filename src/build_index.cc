#include "build_index.h"

#include <iostream>

#include "parasail.h"
#include "parasail/matrices/blosum62.h"
#include "parasail/matrix_lookup.h"

void BuildIndexCommand::run() const
{
    std::cout << "Building index for " << _params.sequences << std::endl;

    const parasail_matrix_t *matrix = parasail_matrix_lookup("blosum62");
    assert(matrix);
}