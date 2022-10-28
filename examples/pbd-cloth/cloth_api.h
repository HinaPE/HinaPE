#ifndef HINAPE_CLOTH_API_H
#define HINAPE_CLOTH_API_H

#include "common/logging.h"
#include "cloth-engine/solver/cloth_solver.h"
#include "render-engine/scene/object.h"
#include "render-engine/physics-API/cloth_system_api.h"

class ClothAPI : public HinaPE::ClothSystemAPI
{
public:
    void step(Scene_Object *_scene_object, float dt) override;

public:
    bool solver_prepared = false;
    std::shared_ptr<HinaPE::ClothEngine::ClothSolver> solver_ptr;
};

#endif //HINAPE_CLOTH_API_H
