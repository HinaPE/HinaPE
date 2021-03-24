#ifndef ASSIGNMENTFRAMEWORK__TEST_SCENES_H
#define ASSIGNMENTFRAMEWORK__TEST_SCENES_H

#include "World2D.h"

using namespace W2D;

namespace TEST_SCENE {

    static void scene01(World2D &world) {
        auto rect = std::make_unique<Rectangle>(Eigen::Vector2f(-0.5f, 0.f), 0.1f, 0.1f);
        rect->set_color(0x4d4398).set_draw_mode(LINES);
        auto floor = std::make_unique<Rectangle>(Eigen::Vector2f(0.f, -.9f), 1.f, .01f);
        floor->set_color(0x4d4398).set_draw_mode(TRIANGLES).fix();
        auto l1 = std::make_unique<Line>(Eigen::Vector2f(-.8f, 0.f), Eigen::Vector2f(.8f, 0.f));
        l1->set_color(0x4d4398).set_draw_mode(POINTS);

        world.add(std::move(rect));
        world.add(std::move(floor));
        world.add(std::move(l1));
    }
}

#endif //ASSIGNMENTFRAMEWORK__TEST_SCENES_H
