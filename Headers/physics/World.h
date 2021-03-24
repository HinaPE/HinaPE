#ifndef ASSIGNMENT0_WORLD_H
#define ASSIGNMENT0_WORLD_H

#include "BasicShapes.h"

#include "Eigen/Eigen"

#include <vector>
#include <memory>
#include <utility>


struct Wind2D {
    bool wind_blowing;
    float wind_force;
    Eigen::Vector2f wind_direction;
};

class World2D {
public:
    World2D();

    explicit World2D(Eigen::Vector2f gravity);

    void init();

    void solve();

public:
    std::vector<std::unique_ptr<BasicShape2D> > objects;
public:
    Eigen::Vector2f gravity;
    int interations_per_frame = 25;
    float time_step = 0.01;
    float air_friction = 0.02;
    bool on_somulation = true;
    Wind2D wind;
};

#endif //ASSIGNMENT0_WORLD_H
