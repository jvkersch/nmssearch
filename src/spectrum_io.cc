#include <fstream>
#include <ios>

#include "spectrum_io.h"

void serialize(std::vector<Spectrum>& spectra, const std::string& fname) {
    std::ofstream output (fname, std::ios::out | std::ios::binary);
    cereal::BinaryOutputArchive ar(output);
    ar(spectra);
}

void serialize(Spectrum& spectrum, const std::string& fname) {
    std::vector<Spectrum> spectra = {spectrum};
    serialize(spectra, fname);
}

std::vector<Spectrum> deserialize(const std::string& fname) {
    std::ifstream input (fname, std::ios::in | std::ios::binary);
    cereal::BinaryInputArchive ar(input);
    std::vector<Spectrum> spectra;
    ar(spectra);
    return spectra;
}
