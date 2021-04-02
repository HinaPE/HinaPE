#ifndef ASSIGNMENT_FRAMEWORK_SCENE02_H
#define ASSIGNMENT_FRAMEWORK_SCENE02_H

#include "SceneTEMPLATE.h"

#include "Fireworks.h"

class Scene_sample2D_01 : public SceneTEMPLATE {
public:
    Scene_sample2D_01(unsigned int width, unsigned int height, bool is_2D = false) : SceneTEMPLATE(width, height,
                                                                                                   is_2D) {}

public:

    void init() override;

    void load() override;

    void update() override;

    void render(float delta_time) override;

    void terminate() override;
};

#endif //ASSIGNMENTFRAMEWORK_SCENE02_H
