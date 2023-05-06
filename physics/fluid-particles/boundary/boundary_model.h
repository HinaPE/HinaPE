#ifndef HINAPE_BOUNDARY_MODEL_H
#define HINAPE_BOUNDARY_MODEL_H

#include "math/vector.h"

namespace HinaPE
{
    class BoundaryModel
    {
    public:
        BoundaryModel() = default;
        virtual ~BoundaryModel() = default;
        virtual void update() = 0;

        struct{
            std::vector<mVector3> forcePerThread;
            std::vector<mVector3> torquePerThread;
        } ForceAndTorque;

    public:
        virtual void reset();
        virtual void performNeighborhoodSearchSort() {};

        void addForce(const mVector3 &pos, const mVector3 &f, const mVector3 &RigidBodyPos);
        void getForceAndTorque(mVector3 &force, mVector3 &torque);
        void clearForceAndTorque();
    };
}
#endif //HINAPE_BOUNDARY_MODEL_H
