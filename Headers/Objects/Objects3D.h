#ifndef ASSIGNMENT_FRAMEWORK_OBJECTS3D_H
#define ASSIGNMENT_FRAMEWORK_OBJECTS3D_H

#include "Shapes3D.h"

#include "PhysicalObjects.h"

class Ground : public Plane, public BasicPhysicalObjects {
public:
    Ground(float width, float height);

    void update() override;

    void draw() override;

    void set_vp(const Eigen::Matrix4f &view, Eigen::Matrix4f projection) override;

    Eigen::Matrix4f get_model() override;

public:

    float width;
    float height;
};


class Ball : public Sphere, public BasicPhysicalObjects {
public:
    explicit Ball(float radius);

    void update() override;

    void draw() override;

    void set_vp(const Eigen::Matrix4f &view, Eigen::Matrix4f projection) override;

    Eigen::Matrix4f get_model() override;

public:

    float radius;
};

#endif //ASSIGNMENT_FRAMEWORK_OBJECTS3D_H
