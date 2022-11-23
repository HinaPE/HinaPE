#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/api.h"
#include "renderer/scene/scene.h"
#include "cloth/solver/cloth_geometry_data.h"

namespace HinaPE::Cloth
{
class Api : public Kasumi::Api, public std::enable_shared_from_this<Api>
{
public:
    void step(Kasumi::Workbench::ScenePtr &scene, float dt) final;
    void gui(Kasumi::Workbench::ScenePtr &scene) final;

private:
    ClothGeometryDataPtr _cloth_data;
};
}

#endif //HINAPE_API_H
