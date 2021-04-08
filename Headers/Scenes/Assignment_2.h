#ifndef ASSIGNMENT_FRAMEWORK_ASSIGNMENT_2_H
#define ASSIGNMENT_FRAMEWORK_ASSIGNMENT_2_H

#include "SceneTEMPLATE.h"

class Assignment_2 : public SceneTEMPLATE {
public:
    Assignment_2(unsigned int width, unsigned int height, bool is_2D = false) : SceneTEMPLATE(width, height, is_2D) {}

    void init() override;

    void load() override;

    void update() override;

    void render(float delta_time) override;

    void terminate() override;
};

#endif //ASSIGNMENT_FRAMEWORK_ASSIGNMENT_2_H
