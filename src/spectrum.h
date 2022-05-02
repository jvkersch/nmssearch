#ifndef _SPECTRUM_H_
#define _SPECTRUM_H_

#include <vector>
#include <map>
#include <string>

class Spectrum {
public:
    Spectrum(const std::string& sequence, int k);
    int size() const;

    // Convert spectrum to map of actual kmers and counts.
    std::map<std::string, size_t> to_map() const;
    
private:
    int m_k;

    std::vector<uint64_t> m_unique;
    std::vector<int> m_counts;
};

#endif // _SPECTRUM_H_