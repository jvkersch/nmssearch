#ifndef _SPACE_H_
#define _SPACE_H_

#include <string>

// nmslib
#include "space/space_string.h"
#include "global.h"

class NeedlemanWunschSpace : public similarity::StringSpace<int>
{
public:
    explicit NeedlemanWunschSpace() {}
    virtual ~NeedlemanWunschSpace() {}

    virtual std::string StrDesc() const;

protected:
    virtual int HiddenDistance(const similarity::Object *obj1, const similarity::Object *obj2) const;

    DISABLE_COPY_AND_ASSIGN(NeedlemanWunschSpace);
};

#endif // _SPACE_H_