#include "PhysicalObjects.h"

BasicPhysicalObjects::BasicPhysicalObjects() : x(Eigen::Vector3f::Zero()), v(Eigen::Vector3f::Zero()),
                                               f(Eigen::Vector3f::Zero()), ff(0.f), m(1.f), is_fixed(false) {}


void BasicPhysicalObjects::set_position(const Eigen::Vector3f &pos) {
    this->x = pos;
}