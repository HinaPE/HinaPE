#ifndef HINAPE_FIELD3_H_
#define HINAPE_FIELD3_H_

#include <memory>

namespace HinaPE
{

//! Abstract base class for 3-D fields.
class Field3
{
public:
    Field3();

    virtual ~Field3();
};

typedef std::shared_ptr<Field3> Field3Ptr;

}  // namespace HinaPE

#endif  // HINAPE_FIELD3_H_
