#ifndef _SPECTRUM_LOADER_H_
#define _SPECTRUM_LOADER_H_

#include <cassert>
#include <string>
#include <vector>

#include "spectrum.h"
#include "parasail/io.h"


inline std::vector<Spectrum> LoadSpectraFromFile(std::string filename, size_t k)
{
    auto sequences = parasail_sequences_from_file(filename.c_str());
    assert(sequences != nullptr);  // TODO: Proper error handling
    std::vector<Spectrum> spectra;

    for (size_t i = 0; i < sequences->l; i++)
    {
        parasail_sequence_t seq = sequences->seqs[i];
        std::string sequence(seq.seq.s, seq.seq.l);
        std::string name(seq.name.s, seq.name.l);
        spectra.push_back(Spectrum(name, sequence, k));
    }
 
    parasail_sequences_free(sequences);
    return spectra;
}

#endif // _SPECTRUM_LOADER_H_
