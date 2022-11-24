#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/api.h"
#include "renderer/scene/scene.h"
#include "cloth/solver/cloth_solver.h"

namespace HinaPE::Cloth
{
class Api : public Kasumi::Api, public std::enable_shared_from_this<Api>
{
public:
    void step(float dt) final;
    void ui_sidebar() final;
    void sync();

private:
    std::map<unsigned int, ClothSolverPtr> _solvers;
};
}

#endif //HINAPE_API_H
