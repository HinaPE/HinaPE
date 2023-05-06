
#include "boundary_akinci2012.h"

void HinaPE::BoundaryModel_Akinci2012::resize() {
    BoundaryBasic.positions.resize(BoundaryBasic.positions_origin.size());
    BoundaryBasic.velocity.resize(BoundaryBasic.positions_origin.size());
    BoundaryBasic.volume.resize(BoundaryBasic.positions_origin.size());
    BoundaryBasic.mass.resize(BoundaryBasic.positions_origin.size());
}

void HinaPE::BoundaryModel_Akinci2012::reset() {
    BoundaryModel::reset();
    for (int i = 0; i < BoundaryBasic.positions.size(); i++)
    {
        BoundaryBasic.positions[i] = BoundaryBasic.positions_origin[i];
        BoundaryBasic.velocity[i] = mVector3(0);
        BoundaryBasic.volume[i] = 0;
        BoundaryBasic.mass[i] = 0;
    }
}

void HinaPE::BoundaryModel_Akinci2012::initModel(const unsigned int numBoundaryParticles, mVector3 *boundaryParticles)
{
    BoundaryBasic.positions_origin.resize(numBoundaryParticles);
    resize();
    for (int i = 0; i < numBoundaryParticles; i++)
    {
        BoundaryBasic.positions_origin[i] = boundaryParticles[i];
        BoundaryBasic.positions[i] = boundaryParticles[i];
    }

}

void HinaPE::BoundaryModel_Akinci2012::computeBoundaryVolume()
{
    for (int i = 0; i < (int)BoundaryBasic.positions.size(); i++)
    {

    }
}



