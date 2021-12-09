#include "nw_space.h"

std::string NeedlemanWunschSpace::StrDesc() const
{
    return "Needleman-Wunsch distance";
}

static int nw(const char *x, size_t xlen, const char *y, size_t ylen)
{
    return 5;
}

int NeedlemanWunschSpace::HiddenDistance(const similarity::Object *obj1, const similarity::Object *obj2) const
{
    CHECK(obj1->datalength() > 0);
    CHECK(obj2->datalength() > 0);
    const char *x = reinterpret_cast<const char *>(obj1->data());
    const char *y = reinterpret_cast<const char *>(obj2->data());
    const size_t len1 = obj1->datalength() / sizeof(char);
    const size_t len2 = obj2->datalength() / sizeof(char);

    return nw(x, len1, y, len2);
}