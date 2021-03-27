#ifndef ASSIGNMENT_FRAMEWORK_SCENE02_H
#define ASSIGNMENT_FRAMEWORK_SCENE02_H

#include "SceneTEMPLATE.h"

class Scene02 : public SceneTEMPLATE {
public:
    Scene02(unsigned int width, unsigned int height, bool is_2D = false) : SceneTEMPLATE(width, height, is_2D) {}
public:

    void init() override;

    void load() override;

    void update() override;

    void render(float delta_time) override;

    void terminate() override;
};

#endif //ASSIGNMENTFRAMEWORK_SCENE02_H
