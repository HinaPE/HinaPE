#ifndef HINAPE_VALIDATIONABLE_H
#define HINAPE_VALIDATIONABLE_H

#include "common.h"

class Validationable
{
protected:
    FORCE_INLINE virtual void validation_check() final
    {
#ifdef _DEBUG
        _validation_check();
#endif
    }

private:
    virtual void _validation_check() = 0;
};

#endif //HINAPE_VALIDATIONABLE_H
