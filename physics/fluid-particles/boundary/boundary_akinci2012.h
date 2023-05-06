#ifndef HINAPE_BOUNDARY_AKINCI2012_H
#define HINAPE_BOUNDARY_AKINCI2012_H

#include "boundary_model.h"

namespace HinaPE
{
    class BoundaryModel_Akinci2012 : public BoundaryModel
    {
        public:
            BoundaryModel_Akinci2012() = default;
            ~BoundaryModel_Akinci2012() override = default;
        public:
            struct{
                bool isSorted;
                unsigned int pointSetIndex; // use for distinguish different boundary

                std::vector<mVector3> 	positions;
                std::vector<mVector3> 	positions_origin;

                std::vector<mVector3>		velocity;
                std::vector<real> 	    volume;

                std::vector<real>		mass;
            } BoundaryBasic;
        public:
            void computeBoundaryVolume();
            void resize();
            void reset() override;

            void initModel(unsigned int numBoundaryParticles, mVector3 *boundaryParticles);
    };
}
#endif //HINAPE_BOUNDARY_AKINCI2012_H
