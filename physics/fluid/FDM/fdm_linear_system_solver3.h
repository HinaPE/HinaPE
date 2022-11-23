#ifndef HINAPE_FDM_LINEAR_SYSTEM_SOLVER3_H
#define HINAPE_FDM_LINEAR_SYSTEM_SOLVER3_H
#include "fdm_linear_system3.h"

#include <memory>

namespace HinaPE {

//! Abstract base class for 3-D finite difference-type linear system solver.
    class FdmLinearSystemSolver3 {
    public:
        FdmLinearSystemSolver3() = default;

        virtual ~FdmLinearSystemSolver3() = default;

        //! Solves the given linear system.
        virtual bool solve(FdmLinearSystem3* system) = 0;

        //! Solves the given compressed linear system.
        virtual bool solveCompressed(FdmCompressedLinearSystem3*) { return false; }
    };

//! Shared pointer type for the FdmLinearSystemSolver3.
    typedef std::shared_ptr<FdmLinearSystemSolver3> FdmLinearSystemSolver3Ptr;

}

#endif //HINAPE_FDM_LINEAR_SYSTEM_SOLVER3_H
