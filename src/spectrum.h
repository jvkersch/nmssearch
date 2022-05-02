#ifndef _SPECTRUM_H_
#define _SPECTRUM_H_

#include <vector>
#include <map>
#include <string>

class Spectrum {
public:
    Spectrum(const std::string& sequence, int k);
    
    int size() const;
    double norm() const;

    // Convert spectrum to map of actual kmers and counts.
    std::map<std::string, size_t> to_map() const;
    
    friend double cosine_distance(const Spectrum& s1, const Spectrum& s2);

private:
    int m_k;
    mutable double m_norm = -1;

    std::vector<uint64_t> m_unique;
    std::vector<int> m_counts;
};

#endif // _SPECTRUM_H_