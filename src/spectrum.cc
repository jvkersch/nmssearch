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

Spectrum::Spectrum(const std::string &sequence, int k)
{
    auto kmers = count_seq(sequence, k);

    if (!kmers.empty())
    {
        std::sort(kmers.begin(), kmers.end());

        unique.push_back(kmers[0]);
        counts.push_back(1);

        for (size_t i = 1; i < kmers.size(); i++)
        {
            if (kmers[i] == unique.back())
            {
                counts.back() += 1;
            }
            else
            {
                unique.push_back(kmers[i]);
                counts.push_back(1);
            }
        }
    }
}

int Spectrum::size() const
{
    return unique.size();
}