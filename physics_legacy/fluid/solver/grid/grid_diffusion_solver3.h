//
// Created by 86151 on 2022/11/3.
//

#ifndef HINAPE_GRID_DIFFUSION_SOLVER3_H
#define HINAPE_GRID_DIFFUSION_SOLVER3_H

#include "../../../../common/grid/collocated_vector_grid3.h"
#include "../../../../common/field/constant_scalar_field3.h"
#include "constants.h"
#include "../../../../common/grid/face_centered_grid3.h"
#include "../../../../common/grid/scalar_grid3.h"
#include <limits>
#include <memory>

namespace HinaPE::Fluid {

//!
//! \brief Abstract base class for 3-D grid-based diffusion equation solver.
//!
//! This class provides functions to solve the diffusion equation for different
//! types of fields. The target equation can be written as
//! \f$\frac{\partial f}{\partial t} = \mu\nabla^2 f\f$ where \f$\mu\f$ is
//! the diffusion coefficient. The field \f$f\f$ can be either scalar or vector
//! field.
//!

    class GridDiffusionSolver3 {
    public:
        //! Default constructor.
        GridDiffusionSolver3();

        //! Default destructor.
        virtual ~GridDiffusionSolver3();

        //!
        //! Solves diffusion equation for a scalar field.
        //!
        //! \param source Input scalar field.
        //! \param diffusionCoefficient Amount of diffusion.
        //! \param timeIntervalInSeconds Small time-interval that diffusion occur.
        //! \param dest Output scalar field.
        //! \param boundarySdf Shape of the solid boundary that is empty by default.
        //! \param boundarySdf Shape of the fluid boundary that is full by default.
        //!
        virtual void solve(
                const ScalarGrid3& source,
                double diffusionCoefficient,
                double timeIntervalInSeconds,
                ScalarGrid3* dest,
                const ScalarField3& boundarySdf = ConstantScalarField3(kMaxD),
                const ScalarField3& fluidSdf = ConstantScalarField3(-kMaxD)) = 0;

        //!
        //! Solves diffusion equation for a collocated vector field.
        //!
        //! \param source Input collocated vector field.
        //! \param diffusionCoefficient Amount of diffusion.
        //! \param timeIntervalInSeconds Small time-interval that diffusion occur.
        //! \param dest Output collocated vector field.
        //! \param boundarySdf Shape of the solid boundary that is empty by default.
        //! \param boundarySdf Shape of the fluid boundary that is full by default.
        //!
        virtual void solve(
                const CollocatedVectorGrid3& source,
                double diffusionCoefficient,
                double timeIntervalInSeconds,
                CollocatedVectorGrid3* dest,
                const ScalarField3& boundarySdf = ConstantScalarField3(kMaxD),
                const ScalarField3& fluidSdf = ConstantScalarField3(-kMaxD)) = 0;

        //!
        //! Solves diffusion equation for a face-centered vector field.
        //!
        //! \param source Input face-centered vector field.
        //! \param diffusionCoefficient Amount of diffusion.
        //! \param timeIntervalInSeconds Small time-interval that diffusion occur.
        //! \param dest Output face-centered vector field.
        //! \param boundarySdf Shape of the solid boundary that is empty by default.
        //! \param boundarySdf Shape of the fluid boundary that is full by default.
        //!
        virtual void solve(
                const FaceCenteredGrid3& source,
                double diffusionCoefficient,
                double timeIntervalInSeconds,
                FaceCenteredGrid3* dest,
                const ScalarField3& boundarySdf = ConstantScalarField3(kMaxD),
                const ScalarField3& fluidSdf = ConstantScalarField3(-kMaxD)) = 0;
    };

//! Shared pointer type for the GridDiffusionSolver3.
    typedef std::shared_ptr<GridDiffusionSolver3> GridDiffusionSolver3Ptr;

}

#endif //HINAPE_GRID_DIFFUSION_SOLVER3_H
