#ifndef ASSIGNMENT_FRAMEWORK_SCENE_H
#define ASSIGNMENT_FRAMEWORK_SCENE_H

#include "Eigen/Eigen"

#include <vector>

#include "PhysicalObjects.h"

class SceneTEMPLATE {
public:
    virtual ~SceneTEMPLATE () = default;

    virtual void init() = 0;

    virtual void load() = 0;

    virtual void update() = 0;

    virtual void render() = 0;

    virtual void terminate() = 0;
};

#endif //ASSIGNMENTFRAMEWORK_SCENE_H
