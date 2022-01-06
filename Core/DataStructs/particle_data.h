#ifndef HINAPE_PARTICLE_DATA_H
#define HINAPE_PARTICLE_DATA_H

#include "Core/Interfaces/define.h"

namespace HinaPE
{
    class ParticleData
    {
    public: // ======================================== PARTICLE DATA CONSTRUCTIONS ========================================

        ParticleData() = default;

        ParticleData(HMatrixXr &points);

        virtual ~ParticleData() = default;

    public:

        /// World Position
        HMatrixXr x_world_;

        /// Local Position
        HMatrixXr x_local_;

        /// Pre Position
        HMatrixXr x_pre_;

        /// Linear Velocity
        HMatrixXr v_linear_;

        /// Linear Angular Velocity
        HMatrixXr a_linear_;

        /// Mass
        HMatrixXr mass_;

        /// Inv Mass
        HMatrixXr mass_inv_;

    private:

        void build();
    };
}


#endif //HINAPE_PARTICLE_DATA_H
