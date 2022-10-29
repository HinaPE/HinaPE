#ifndef HINAPE_CLOTH_API_H
#define HINAPE_CLOTH_API_H

#include "common/logging.h"
#include "cloth-engine/solver/cloth_solver.h"
#include "render-engine/scene/object.h"
#include "render-engine/physics-system-api.h"

class ClothAPI : public HinaPE::ClothSystemAPI
{
public:
    void step(Scene_Particles *_scene_particles, float dt) override;
    void gui(Gui::Manager &manager, Scene &scene, Undo &undo, Gui::Widgets &widgets, Scene_Maybe obj, int &index) override;

public:
    bool _solver_prepared = false;
    std::shared_ptr<HinaPE::ClothEngine::ClothSolver> _solver_ptr;
};

#endif //HINAPE_CLOTH_API_H
