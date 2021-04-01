#ifndef ASSIGNMENT_FRAMEWORK_HELLOHINA_H
#define ASSIGNMENT_FRAMEWORK_HELLOHINA_H

#include "SceneTEMPLATE.h"

#include "Fireworks.h"

class HelloHina : public SceneTEMPLATE {
public:
    HelloHina(unsigned int width, unsigned int height, bool is_2D = false) : SceneTEMPLATE(width, height, is_2D) {}

    void init() override;

    void load() override;

    void update() override;

    void render(float delta_time) override;

    void terminate() override;

public:
    std::unique_ptr<Fireworks2D> fireworks;
};

#endif //ASSIGNMENT_FRAMEWORK_HELLOHINA_H
