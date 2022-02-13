#ifndef _SPACE_H_
#define _SPACE_H_

#include <string>

// nmslib
#include "space/space_string.h"
#include "global.h"

// local
#include "align.h"

template<typename AlignerCls>
class AlignmentSpace : public similarity::StringSpace<int>
{
public:
    explicit AlignmentSpace() {}
    explicit AlignmentSpace(AlignerCls aligner): m_aligner(aligner) {}
    virtual ~AlignmentSpace() {}

    virtual std::string StrDesc() const;

private:
    AlignerCls m_aligner;

protected:
    virtual int HiddenDistance(const similarity::Object *obj1, const similarity::Object *obj2) const;

    DISABLE_COPY_AND_ASSIGN(AlignmentSpace);
};

// Implementation

template<typename T>
std::string AlignmentSpace<T>::StrDesc() const
{
    return "Needleman-Wunsch distance";
}

template<typename AlignerCls>
int AlignmentSpace<AlignerCls>::HiddenDistance(const similarity::Object *obj1, const similarity::Object *obj2) const
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