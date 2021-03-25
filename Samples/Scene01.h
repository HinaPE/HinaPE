#ifndef ASSIGNMENT_FRAMEWORK_SCENE01_H
#define ASSIGNMENT_FRAMEWORK_SCENE01_H

#include "BasicShapes.h"

#include <vector>

class Scene01 {
public:
    std::vector<std::unique_ptr<BasicShape>> objects;

    void load();
};

#endif //ASSIGNMENTFRAMEWORK_SCENE01_H
