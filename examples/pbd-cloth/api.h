#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "../../kasumi/workbench/api.h"
#include "scene/scene.h"
#include "solver/cloth_geometry_data.h"

namespace HinaPE::ClothEngine
{
class Api : public Kasumi::Api, public std::enable_shared_from_this<Api>
{
public:
    void step(Kasumi::Workbench::ScenePtr &scene, float dt) final;
    void gui(Kasumi::Workbench::ScenePtr &scene) final;
};
}

#endif //HINAPE_API_H
