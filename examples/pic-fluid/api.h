#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/api.h"
#include "renderer/scene/scene.h"
#include "fluid/solver/pic/pic_solver3.h"
#include "fluid/emitter/volume_particle_emitter3.h"
#include "fluid/emitter/particle_emitter_set3.h"
#include "fluid/point_generator/grid_point_generator3.h"
#include "common/geometry/bounding_box3.h"
#include "common/geometry/plane3.h"
#include "common/geometry/box3.h"
#include "common/geometry/cylinder3.h"
#include "common/geometry/rigid_body_collider3.h"
#include "common/geometry/sphere3.h"
#include "common/geometry/implicit_surface_set3.h"



namespace HinaPE::Fluid
{
    class Api : public Kasumi::Api
    {
    public:
        void step(float dt) final;
        void ui_sidebar() final;
        void sync();
    public:
        void assign_particles_domain(const HinaPE::BoundingBox3D &);
        HinaPE::BoundingBox3D _particles_domain;
    private:
        std::map<Kasumi::TexturedMeshPtr, PicSolver3Ptr> _solvers;
    };
}

#endif //HINAPE_API_H
