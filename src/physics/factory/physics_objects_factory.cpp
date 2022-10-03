#include "physics_objects_factory.h"

#include <algorithm>

auto HinaPE::RigidBodyFactory::create_rigidbody(HinaPE::RigidBodyType type) -> HinaPE::PhysicsObjectPtr
{
    return std::make_shared<HinaPE::PhysicsObject>(HinaPE::PhysicsObjectType::Rigidbody); // TODO: to support change types
}

auto HinaPE::ClothFactory::create_cloth(const HinaPE::ClothFactory::ClothDesc &desc) -> std::shared_ptr<HinaPE::PhysicsObject>
{
    DeformableBase<CLOTH> cloth;

    auto &_p = cloth.impl->p;
    auto &_r = cloth.impl->q;

    auto &_ps = cloth.impl->positions;
    auto &_vs = cloth.impl->velocities;
    auto &_ms = cloth.impl->masses;
    auto &_ims = cloth.impl->inv_masses;
    auto &_is = cloth.impl->indices;
    auto &_ss = cloth.impl->stiffness;

    auto width = desc.width;
    auto height = desc.height;
    auto row = desc.row;
    auto col = desc.col;

    auto position = desc.position;
    auto rotation = desc.rotation;

    auto delta_x = (float) width / (float) col;
    auto delta_y = (float) height / (float) row;
    auto bias_x = (float) width / 2.f;
    auto bias_y = (float) height / 2.f;

    _ps.reserve(row * col);
    _vs.reserve(row * col);
    _ms.reserve(row * col);
    _ims.reserve(row * col);
    _is.reserve(6 * (row - 1) * (col - 1));

    for (int j = 0; j < row; ++j)
    {
        for (int i = 0; i < col; ++i)
        {
            Vec3 x((float) i * delta_x - bias_x, (float) j * delta_y - bias_y, 0.f);
            x += position;
            _ps.push_back(x);
        }
    }

    for (int i = 0; i < col - 1; ++i)
    {
        for (int j = 0; j < row - 1; ++j)
        {
            // counter-clockwise indices to form a (front-faced) triangle
            if ((i + j) % 2 == 0)
            {
                _is.push_back(j * col + i);
                _is.push_back((j + 1) * col + i);
                _is.push_back(j * col + i + 1);

                _is.push_back((j + 1) * col + i + 1);
                _is.push_back(j * col + i + 1);
                _is.push_back((j + 1) * col + i);
            } else
            {
                _is.push_back(j * col + i);
                _is.push_back((j + 1) * col + i + 1);
                _is.push_back(j * col + i + 1);

                _is.push_back(j * col + i);
                _is.push_back((j + 1) * col + i);
                _is.push_back((j + 1) * col + i + 1);
            }

        }
    }

    std::fill(_vs.begin(), _vs.end(), Vec3(0.f, 0.f, 0.f));
    std::fill(_ms.begin(), _ms.end(), desc.mass);
    std::fill(_ims.begin(), _ims.end(), 1.f / desc.mass);

    _p = position;
    //    _r = rotation; TODO: fix here
    _ss = desc.stiffness;

    cloth.setup_geometry();

    return std::make_shared<PhysicsObject>(std::move(cloth));
}
