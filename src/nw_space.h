#ifndef _SPACE_H_
#define _SPACE_H_

#include <string>

// nmslib
#include "space/space_string.h"
#include "global.h"

// local
#include "align.h"

class NeedlemanWunschSpace : public similarity::StringSpace<int>
{
public:
    explicit NeedlemanWunschSpace() {}
    virtual ~NeedlemanWunschSpace() {}

    virtual std::string StrDesc() const;

private:
    Aligner _aligner; // TODO: Configure this external and inject into space.

protected:
    virtual int HiddenDistance(const similarity::Object *obj1, const similarity::Object *obj2) const;

    DISABLE_COPY_AND_ASSIGN(NeedlemanWunschSpace);
};

#endif // _SPACE_H_