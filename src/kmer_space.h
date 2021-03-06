#ifndef _KMER_SPACE_H_
#define _KMER_SPACE_H_

#include <string>

// nmslib
#include "space/space_string.h"
#include "global.h"

// local
#include "spectrum.h"


class KMerSpace : public similarity::StringSpace<float>
{
public:
    explicit KMerSpace(const std::vector<Spectrum> *spectra) : m_spectra(spectra) {}
    virtual ~KMerSpace() {}

    virtual std::string StrDesc() const;

protected:
    const std::vector<Spectrum> *m_spectra = nullptr;

    virtual float HiddenDistance(const similarity::Object *obj1, const similarity::Object *obj2) const;

    DISABLE_COPY_AND_ASSIGN(KMerSpace);
};

#endif // _SPACE_H_