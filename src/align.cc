#include "align.h"

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

double Aligner::align_stats(const char *s1, size_t len1, const char *s2, size_t len2) const
{
    parasail_result_t *result = parasail_nw_stats_scan_16(s1, len1, s2, len2, 12, 4, &parasail_nuc44);
    // int score = parasail_result_get_score(result);
    double dist = 100.0 * parasail_result_get_matches(result) / parasail_result_get_length(result);
    parasail_result_free(result);

    return dist; // TOOD Compute and return other alignment stats
}