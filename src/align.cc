#include "align.h"
#include "sequence_reader.h" // TODO factor out Sequence class

#include <parasail.h>
#include <parasail/matrices/nuc44.h>

Aligner::Aligner()
{
}

int Aligner::align(const char *s1, size_t len1, const char *s2, size_t len2) const
{
    parasail_result_t *result = parasail_nw_scan_16(s1, len1, s2, len2, 12, 4, &parasail_nuc44);
    int score = parasail_result_get_score(result);
    parasail_result_free(result);

    return 5 * len1 + 5 * len2 - 2 * score;
}

AlignStats Aligner::align_stats(const FASTASequence &seq1, const FASTASequence &seq2) const
{
    auto s1 = seq1.sequence;
    auto s2 = seq2.sequence;

    parasail_result_t *result = parasail_nw_stats_scan_16(
        s1.c_str(), s1.size(), s2.c_str(), s2.size(), 12, 4, &parasail_nuc44);
    int length = parasail_result_get_length(result);
    int score = parasail_result_get_score(result);

    double pctIdentity = 100.0 * parasail_result_get_matches(result) / length;
    parasail_result_free(result);

    // TODO: fill in other results
    return {
        seq1.name,
        seq2.name,
        pctIdentity,
        length,
        0,
        0,
        1,
        s1.size(),
        1,
        s2.size(),
        0.0,
        score
    };
}