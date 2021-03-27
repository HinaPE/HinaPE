#ifndef ASSIGNMENT_FRAMEWORK_SCENE01_H
#define ASSIGNMENT_FRAMEWORK_SCENE01_H

#include "SceneTEMPLATE.h"

class Scene01 : public SceneTEMPLATE {
public:
    Scene01(unsigned int width, unsigned int height) : SceneTEMPLATE(width, height) {}
public:

    void init() override;

    void load() override;

    void update() override;

    void render(float delta_time) override;

    void terminate() override;
};

#endif //ASSIGNMENTFRAMEWORK_SCENE01_H
