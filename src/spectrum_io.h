#ifndef _SPECTRUM_IO_H_
#define _SPECTRUM_IO_H_

#include <string>
#include <vector>

#include "spectrum.h"

void serialize(Spectrum& spectrum, const std::string& fname);
void serialize(std::vector<Spectrum>& spectra, const std::string& fname);
std::vector<Spectrum> deserialize(const std::string& fname);

#endif // _SPECTRUM_IO_H_