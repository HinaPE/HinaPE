#include "particle_data.h"

using namespace HinaPE;

ParticleData::ParticleData(HMatrixXr &points)
{
    x_world_ = points;
    x_local_ = points;
    build();
}

void ParticleData::build()
{

}
