#ifndef ASSIGNMENT_FRAMEWORK_ASSIGNMENT_3_H
#define ASSIGNMENT_FRAMEWORK_ASSIGNMENT_3_H

#include "SceneTEMPLATE.h"

#include "MassSpringTEMPLATE.h"

class Assignment_3 : public SceneTEMPLATE {
public:
    Assignment_3(unsigned int width, unsigned int height, bool is_2D = true) : SceneTEMPLATE(width, height, is_2D) {}

    void init() override;

    void load() override;

    void update() override;

    void render(float delta_time) override;

    void terminate() override;

    std::unique_ptr<MassSpringTEMPLATE> mass_spring_system;
};

#endif //ASSIGNMENT_FRAMEWORK_ASSIGNMENT_3_H
