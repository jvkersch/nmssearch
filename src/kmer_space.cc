#include "kmer_space.h"

std::string KMerSpace::StrDesc() const
{
    return "KMer cosine distance";
}

float KMerSpace::HiddenDistance(const similarity::Object *obj1, const similarity::Object *obj2) const
{
    auto s1 = (*m_spectra)[obj1->id()], s2 = (*m_spectra)[obj2->id()];
    return cosine_distance(s1, s2);
}
