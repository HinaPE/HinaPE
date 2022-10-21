#ifndef SRC_JET_FACTORY_H_
#define SRC_JET_FACTORY_H_

#include "grid/scalar_grid2.h"
#include "grid/scalar_grid3.h"
#include "grid/vector_grid2.h"
#include "grid/vector_grid3.h"
#include "searcher/point_neighbor_searcher2.h"
#include "searcher/point_neighbor_searcher3.h"

#include <string>

namespace jet
{

class Factory
{
public:
    static ScalarGrid2Ptr buildScalarGrid2(const std::string &name);

    static ScalarGrid3Ptr buildScalarGrid3(const std::string &name);

    static VectorGrid2Ptr buildVectorGrid2(const std::string &name);

    static VectorGrid3Ptr buildVectorGrid3(const std::string &name);

    static PointNeighborSearcher2Ptr buildPointNeighborSearcher2(const std::string &name);

    static PointNeighborSearcher3Ptr buildPointNeighborSearcher3(const std::string &name);
};

}  // namespace jet

#endif  // SRC_JET_FACTORY_H_
