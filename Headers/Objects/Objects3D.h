#ifndef ASSIGNMENT_FRAMEWORK_OBJECTS3D_H
#define ASSIGNMENT_FRAMEWORK_OBJECTS3D_H

#include "Shapes3D.h"

#include "PhysicalObjects.h"

class Box : public Cube, public BasicPhysicalObjects {
public:
    Box(float width, float height, float depth);

    void update() override;

    void draw() override;

    void set_vp(const Eigen::Matrix4f &view, Eigen::Matrix4f projection) override;

    Eigen::Matrix4f get_model() override;

    std::shared_ptr<AABB> &get_aabb() override;

public:

    float width;
    float height;
    float depth;
};

class Ball : public Sphere, public BasicPhysicalObjects {
public:
    explicit Ball(float radius);

    void update() override;

    void draw() override;

    void set_vp(const Eigen::Matrix4f &view, Eigen::Matrix4f projection) override;

    std::shared_ptr<AABB> &get_aabb() override;

    Eigen::Matrix4f get_model() override;

public:

    float radius;
};

#endif //ASSIGNMENT_FRAMEWORK_OBJECTS3D_H
