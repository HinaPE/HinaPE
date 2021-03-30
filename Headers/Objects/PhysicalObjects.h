#ifndef ASSIGNMENTFRAMEWORK_PHYSICALOBJECTS_H
#define ASSIGNMENTFRAMEWORK_PHYSICALOBJECTS_H

#include "BasicShapes.h"

#include "Eigen/Eigen"

class BasicPhysicalObjects {
public:
    BasicPhysicalObjects();

    virtual ~BasicPhysicalObjects() = default;

public:
    bool is_fixed;

public:
    Eigen::Vector3f x; // centroid position
    Eigen::Vector3f v; // velocity
    Eigen::Vector3f f; // force
    float ff; // friction
    float m; // mass
    Eigen::Matrix4f rotation; // force

public:
    virtual void update() = 0;

    virtual void draw() = 0;

    void rotate(float x_axis, float y_axis, float z_axis);

    void set_position(const Eigen::Vector3f &pos);

    virtual void set_vp(const Eigen::Matrix4f &view, Eigen::Matrix4f projection) = 0;

    virtual void set_ortho(const Eigen::Matrix4f &ortho) {}

    virtual Eigen::Matrix4f get_model() = 0;

    virtual std::shared_ptr<AABB> &get_aabb() {};
};


#endif //ASSIGNMENTFRAMEWORK_PHYSICALOBJECTS_H
