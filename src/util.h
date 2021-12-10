#ifndef _UTIL_H_
#define _UTIL_H_

// nmslib
#include "object.h"

// local
#include "nw_space.h"
#include "sequence_reader.h"

// TODO: Not in the right place, not the right abstraction

similarity::ObjectVector CollectSequences(FASTASequenceReader &reader, const NeedlemanWunschSpace &space);

#endif // _UTIL_H_