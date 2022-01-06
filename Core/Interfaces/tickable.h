#ifndef HINAPE_TICKABLE_H
#define HINAPE_TICKABLE_H

#include "define.h"

class Tickable
{
public:

    virtual void tick(Real dt) = 0;
};

#endif //HINAPE_TICKABLE_H
