#ifndef INCLUDE_JET_FIELD3_H_
#define INCLUDE_JET_FIELD3_H_

#include <memory>

namespace jet
{

//! Abstract base class for 3-D fields.
class Field3
{
public:
    Field3();

    virtual ~Field3();
};

typedef std::shared_ptr<Field3> Field3Ptr;

}  // namespace jet

#endif  // INCLUDE_JET_FIELD3_H_
