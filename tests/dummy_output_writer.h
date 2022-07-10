#ifndef _DUMMY_OUTPUT_WRITER_
#define _DUMMY_OUTPUT_WRITER_

#include "output_writer.h"


class DummyOutputWriter: public BaseOutputWriter {
public:
    void display(const AlignStats &r, std::ostream& output) const override {
        m_stats.push_back(r);
    }
    void display(const std::vector<AlignStats>& results, std::ostream& output) const override {
        m_stats.reserve(m_stats.size() + results.size());
        m_stats.insert(m_stats.end(), results.begin(), results.end());
    }

    size_t ncalls() const {
        return m_stats.size();
    }

    AlignStats operator[](size_t i) const {
        return m_stats[i];
    }

private:
    mutable std::vector<AlignStats> m_stats;
};

#endif // _DUMMY_OUTPUT_WRITER_