#include "rigidbody.h"

using namespace HinaPE;

#include <tuple>

Rigidbody::Rigidbody(const std::string &name)
{
    assignID();

    TYPE_ = ObjectType::RIGIDBODY_TYPE;

    std::tuple<HMatrixXr, HMatrixXi> res = load_model(name);

    ParticleData pd(std::get<0>(res));
    IndexedFaceMesh mesh(std::get<0>(res), std::get<1>(res));

    pd_ = pd;
    mesh_ = mesh;
}

HMatrixXr &Rigidbody::get_render_particle()
{
    return pd_.x_world_;
}

HMatrixXi &Rigidbody::get_render_mesh()
{
    return mesh_.faces_;
}

void Rigidbody::tick(Real dt)
{

}

void Rigidbody::init_inner_constraints()
{

}
