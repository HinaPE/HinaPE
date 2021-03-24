/**
 * @author Xayah, BFU
 * @date 3.24, 2021
 */
#ifndef ASSIGNMENT_FRAMEWORK_WORLD3D_H
#define ASSIGNMENT_FRAMEWORK_WORLD3D_H

#include "BasicShapes3D.h"
#include "Renderer3D.h"

#include "Eigen/Eigen"

#include <vector>
#include <memory>
#include <utility>

namespace W3D {


    struct Wind3D {
        bool wind_blowing;
        float wind_force;
        Eigen::Vector3f wind_direction;
    };

    class World3D {
    public:
        World3D();

        explicit World3D(Eigen::Vector3f gravity);

        void add(std::unique_ptr<BasicShape3D> &&obj);

        void solve();

        void display();

    private:
        void init();

        std::vector<std::unique_ptr<BasicShape3D> > objects;
        std::unique_ptr<Renderer3D> renderer;

    public:
        Eigen::Vector3f gravity;
        int interations_per_frame = 25;
        float time_step = 0.01;
        float air_friction = 0.02;
        bool on_somulation = true;
        Wind3D wind;
    };

}

#endif //ASSIGNMENTFRAMEWORK_WORLD3D_H
