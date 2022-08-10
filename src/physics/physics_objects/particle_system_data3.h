#ifndef HINAPE_PARTICLE_SYSTEM_DATA3_H
#define HINAPE_PARTICLE_SYSTEM_DATA3_H

#include "../lib/basic.h"
#include "../lib/array1.h"
#include "../util/point_neighbor_searcher3.h"

#include <memory>
#include <vector>

namespace HinaPE
{
class ParticleSystemData3
{
    using ScalarData = Array1<double>;
    using VectorData = Array1<Vector3D>;

private:
    double _radius;
    double _mass;
    size_t _number_of_particles;
    size_t _position_idx;
    size_t _velocity_idx;
    size_t _force_idx;

    std::vector<ScalarData> _scalar_data_list;
    std::vector<VectorData> _vector_data_list;
    PointNeighborSearcher3Ptr _neighbor_searcher;
    std::vector<std::vector<size_t>> _neighborLists;

public:
    ParticleSystemData3();
    explicit ParticleSystemData3(size_t number_of_particles);
    ParticleSystemData3(const ParticleSystemData3 &other);
    virtual ~ParticleSystemData3();
};
using ParticleSystemData3Ptr = std::shared_ptr<ParticleSystemData3>;
}

#endif //HINAPE_PARTICLE_SYSTEM_DATA3_H
