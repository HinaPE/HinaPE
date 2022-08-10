#ifndef HINAPE_PARTICLE_SYSTEM_DATA3_H
#define HINAPE_PARTICLE_SYSTEM_DATA3_H

namespace HinaPE
{
class ParticleSystemData3
{
private:
    double _radius;
    double _mass;
    size_t _number_of_particles;
    size_t _position_idx;
    size_t _velocity_idx;
    size_t _force_idx;

public:
    ParticleSystemData3();
    explicit ParticleSystemData3(size_t number_of_particles);
    ParticleSystemData3(const ParticleSystemData3& other);
    virtual ~ParticleSystemData3();
};
}

#endif //HINAPE_PARTICLE_SYSTEM_DATA3_H
