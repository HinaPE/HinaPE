
#include "boundary_model.h"

void HinaPE::BoundaryModel::reset() {
    for (int j = 0; j < ForceAndTorque.forcePerThread.size(); j++)
    {
        ForceAndTorque.forcePerThread[j] = mVector3(0);
        ForceAndTorque.torquePerThread[j]= mVector3(0);
    }
}

void HinaPE::BoundaryModel::addForce(const mVector3 &pos, const mVector3 &f, const mVector3& RigidBodyPos) {
    for (int j = 0; j < ForceAndTorque.forcePerThread.size(); j++)
    {
        mVector3 r = pos - RigidBodyPos;
        ForceAndTorque.forcePerThread[j] += f;
        ForceAndTorque.torquePerThread[j] += r.cross(f);
    }
}

void HinaPE::BoundaryModel::getForceAndTorque(mVector3 &force, mVector3 &torque) {
    force = mVector3(0);
    torque = mVector3(0);
    for (int j = 0; j < this->ForceAndTorque.forcePerThread.size(); j++)
    {
        force += this->ForceAndTorque.forcePerThread[j];
        torque += this->ForceAndTorque.torquePerThread[j];
    }
}

void HinaPE::BoundaryModel::clearForceAndTorque() {
    for (int j = 0; j < ForceAndTorque.forcePerThread.size(); j++)
    {
        ForceAndTorque.forcePerThread[j] = mVector3(0);
        ForceAndTorque.torquePerThread[j] = mVector3(0);
    }
}



