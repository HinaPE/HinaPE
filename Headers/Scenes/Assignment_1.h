#ifndef ASSIGNMENT_FRAMEWORK_ASSIGNMENT_1_H
#define ASSIGNMENT_FRAMEWORK_ASSIGNMENT_1_H

#include "SceneTEMPLATE.h"

class Assignment_1 : public SceneTEMPLATE {
public:
    Assignment_1(unsigned int width, unsigned int height, bool is_2D = false) : SceneTEMPLATE(width, height, is_2D) {}

    void init() override;

    void load() override;

    void update() override;

    void render(float delta_time) override;

    void terminate() override;
};


#endif //ASSIGNMENT_FRAMEWORK_ASSIGNMENT_1_H
