/**
 * @author Xayah, BFU
 * @date 3.24, 2021
 */
#ifndef ASSIGNMENT_FRAMEWORK_WORLD2D_H
#define ASSIGNMENT_FRAMEWORK_WORLD2D_H

#include "BasicShapes2D.h"
#include "Renderer2D.h"

#include "Eigen/Eigen"

#include <vector>
#include <memory>
#include <utility>

namespace W2D {


    struct Wind2D {
        bool wind_blowing;
        float wind_force;
        Eigen::Vector2f wind_direction;
    };

    class World2D {
    public:
        World2D();

        explicit World2D(Eigen::Vector2f gravity);

        void add(std::unique_ptr<BasicShape2D> &&obj);

        void solve();

        void display();

    private:
        void init();

        std::vector<std::unique_ptr<BasicShape2D> > objects;
        std::unique_ptr<W2D::Renderer2D> renderer;

    public:
        Eigen::Vector2f gravity;
        int interations_per_frame = 25;
        float time_step = 0.01;
        float air_friction = 0.02;
        bool on_somulation = true;
        Wind2D wind;
    };
}


#endif //ASSIGNMENT0_WORLD_H
