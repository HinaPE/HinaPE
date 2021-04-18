#ifndef ASSIGNMENT_FRAMEWORK_ASSIGNMENT_4_H
#define ASSIGNMENT_FRAMEWORK_ASSIGNMENT_4_H

#include "SceneTEMPLATE.h"
#include "Cloth.h"

using namespace CLOTH_SIMULATION;

class Assignment_4 : public SceneTEMPLATE {
public:
    Assignment_4(unsigned int width, unsigned int height, bool is_2D = false) : SceneTEMPLATE(width, height, is_2D) {}

    void init() override;

    void load() override;

    void update() override;

    void render(float delta_time) override;

    void terminate() override;

    std::unique_ptr<Cloth> cloth;
};

#endif //ASSIGNMENT_FRAMEWORK_ASSIGNMENT_4_H
