#ifndef ASSIGNMENT_FRAMEWORK_SCENE03_H
#define ASSIGNMENT_FRAMEWORK_SCENE03_H

#include "SceneTEMPLATE.h"

#include "Fireworks.h"

class Scene03 : public SceneTEMPLATE {
public:
    void init() override;

    void load() override;

    void update() override;

    void render(float delta_time) override;

    void terminate() override;
};

#endif //ASSIGNMENT_FRAMEWORK_SCENE03_H
