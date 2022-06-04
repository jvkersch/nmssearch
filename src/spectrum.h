#ifndef _SPECTRUM_H_
#define _SPECTRUM_H_

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>

#include <ostream>
#include <map>
#include <string>
#include <vector>

class Spectrum {
public:
    Spectrum() {};
    Spectrum(const std::string& sequence, int k);
    Spectrum(const std::string& name, const std::string& sequence, int k);
    
    int size() const;
    double norm() const;

    std::string name() const;

    bool operator==(const Spectrum& other) const;
    bool operator!=(const Spectrum& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Spectrum& s);

    // Convert spectrum to map of actual kmers and counts.
    std::map<std::string, size_t> to_map() const;
    
    friend double cosine_distance(const Spectrum& s1, const Spectrum& s2);

    // Serialization/deserialization support
    template <class Archive>
    void serialize(Archive& ar) {
        ar(m_k, m_unique, m_counts);
    }

private:
    std::string m_name;
    int m_k;
    mutable double m_norm = -1;

    std::vector<uint64_t> m_unique;
    std::vector<int> m_counts;
};

#endif // _SPECTRUM_H_