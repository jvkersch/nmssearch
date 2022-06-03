#include <algorithm>
#include <cmath>
#include <numeric>
#include <string>
#include <vector>
#include <iostream>

#include "spectrum.h"

// Adapted from https://github.com/lh3/kmer-cnt/blob/master/kc-c1.c

const unsigned char seq_nt4_table[256] = { // translate ACGT to 0123
    0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 0, 4, 1, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 0, 4, 1, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

auto count_seq(const std::string &seq, int k)
{
    std::vector<uint64_t> kmers;

    int i, l;
    uint64_t x[2], mask = (1ULL << k * 2) - 1, shift = (k - 1) * 2;

    size_t len = seq.size();
    for (i = l = 0, x[0] = x[1] = 0; i < len; ++i)
    {
        int c = seq_nt4_table[(uint8_t)seq[i]];
        if (c < 4)
        {                                                  // not an "N" base
            x[0] = (x[0] << 2 | c) & mask;                 // forward strand
            x[1] = x[1] >> 2 | (uint64_t)(3 - c) << shift; // reverse strand
            if (++l >= k)
            { // we find a k-mer
#ifdef KMER_CANONICAL
                uint64_t y = x[0] < x[1] ? x[0] : x[1];
#else
                uint64_t y = x[0];
#endif
                kmers.push_back(y);
            }
        }
        else
            l = 0, x[0] = x[1] = 0; // if there is an "N", restart
    }

    return kmers;
}

std::string expand_kmer(uint64_t kmer, int k)
{
    std::vector<char> seq;
    for (int i = 0; i < k; ++i)
    {
        seq.push_back("ACGT"[kmer & 3]);
        kmer >>= 2;
    }
    return std::string(seq.rbegin(), seq.rend());
}

Spectrum::Spectrum(const std::string &sequence, int k) : m_k(k)
{
    auto kmers = count_seq(sequence, k);

    if (!kmers.empty())
    {
        std::sort(kmers.begin(), kmers.end());

        m_unique.push_back(kmers[0]);
        m_counts.push_back(1);

        for (size_t i = 1; i < kmers.size(); i++)
        {
            if (kmers[i] == m_unique.back())
            {
                m_counts.back() += 1;
            }
            else
            {
                m_unique.push_back(kmers[i]);
                m_counts.push_back(1);
            }
        }
    }
}

int Spectrum::size() const
{
    return m_unique.size();
}

double Spectrum::norm() const
{
    if (m_norm < 0) {
        m_norm = std::accumulate(m_counts.begin(), m_counts.end(), 0.0, [](double sum, double count) {
            return sum + count*count;
        });
        m_norm = sqrt(m_norm);
    }
    return m_norm;
}

bool Spectrum::operator==(const Spectrum& other) const 
{
    return (m_k == other.m_k) && (m_unique == other.m_unique) && (m_counts == other.m_counts);
}

bool Spectrum::operator!=(const Spectrum& other) const
{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Spectrum& s) 
{
    auto kmermap = s.to_map();
    os << '[';
    for (const auto& it: kmermap) {
        os << it.first << "=" << it.second << " ";
    }
    os << ']';
    return os;
}

std::map<std::string, size_t> Spectrum::to_map() const
{
    std::map<std::string, size_t> map;
    for (size_t i = 0; i < m_unique.size(); i++)
    {
        map[expand_kmer(m_unique[i], m_k)] = m_counts[i];
    }
    return map;
}

double cosine_distance(const Spectrum& s1, const Spectrum& s2) {
    int product = 0;
    for (size_t i = 0, j = 0; i < s1.size(); i++) {
        while (j < s2.size() && s2.m_unique[j] < s1.m_unique[i]) {
            j++;
        }
        if (j < s2.size() && s2.m_unique[j] == s1.m_unique[i]) {
            product += s1.m_counts[i] * s2.m_counts[j];
        }
    }

    return 1 - product / s1.norm() / s2.norm();
}