#ifndef HINAPE_DATA_H
#define HINAPE_DATA_H

#include "grid.h"
#include <vector>

namespace Hina
{
class GridSystemData3
{
private:
	std::vector<Hina::ScalarGrid3Ptr> _scalar_data;
	std::vector<Hina::VectorGrid3Ptr> _vector_data;
	std::vector<Hina::ScalarGrid3Ptr> _advectable_scalar_data;
	std::vector<Hina::VectorGrid3Ptr> _advectable_vector_data;
};
using GridSystemData3Ptr = std::shared_ptr<GridSystemData3>;
}

#endif //HINAPE_DATA_H
