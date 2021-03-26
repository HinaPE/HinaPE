#ifndef ASSIGNMENT_FRAMEWORK_SCENE01_H
#define ASSIGNMENT_FRAMEWORK_SCENE01_H

#include "SceneTEMPLATE.h"

class Scene01 : public SceneTEMPLATE {
public:
    Scene01() = default;
public:

    ~Scene01() override;

    void init() override;

    void load() override;

    void update() override;

    void render() override;

    void terminate() override;

    std::vector<std::unique_ptr<BasicPhysicalObjects>> objects;

    Eigen::Matrix4f view, projection;
};

#endif //ASSIGNMENTFRAMEWORK_SCENE01_H
