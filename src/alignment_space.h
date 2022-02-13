#ifndef _SPACE_H_
#define _SPACE_H_

#include <string>

// nmslib
#include "space/space_string.h"
#include "global.h"

template<typename Aligner>
class AlignmentSpace : public similarity::StringSpace<int>
{
public:
    explicit AlignmentSpace(const Aligner &aligner): m_aligner(aligner) {}
    virtual ~AlignmentSpace() {}

    virtual std::string StrDesc() const;

private:
    const Aligner &m_aligner;

protected:
    virtual int HiddenDistance(const similarity::Object *obj1, const similarity::Object *obj2) const;

    DISABLE_COPY_AND_ASSIGN(AlignmentSpace);
};

// Implementation

template<typename Aligner>
std::string AlignmentSpace<Aligner>::StrDesc() const
{
    return "Alignment-based distance";
}

template<typename Aligner>
int AlignmentSpace<Aligner>::HiddenDistance(const similarity::Object *obj1, const similarity::Object *obj2) const
{
    CHECK(obj1->datalength() > 0);
    CHECK(obj2->datalength() > 0);
    const char *x = reinterpret_cast<const char *>(obj1->data());
    const char *y = reinterpret_cast<const char *>(obj2->data());
    const size_t len1 = obj1->datalength() / sizeof(char);
    const size_t len2 = obj2->datalength() / sizeof(char);

    return m_aligner.align(x, len1, y, len2);
}

#endif // _SPACE_H_