#ifndef HINAPE_DATA_H
#define HINAPE_DATA_H

#include "grid.h"
#include <vector>

namespace Hina
{
class GridSystemData3
{
public:
	void resize(const Base::Size3 &resolution, const Base::mVector3 &grid_spacing, const Base::mVector3 &origin);

public:
	struct Opt
	{
		Base::Size3 resolution;
		Base::mVector3 grid_spacing;
		Base::mVector3 origin;
	} _opt;

public:
	explicit GridSystemData3() = default;
	explicit GridSystemData3(const Opt &opt);
	GridSystemData3(const GridSystemData3 &other) = delete;
	auto operator=(const GridSystemData3 &other) -> GridSystemData3 & = delete;
	GridSystemData3(GridSystemData3 &&other) = delete;
	auto operator=(GridSystemData3 &&other) -> GridSystemData3 & = delete;
	~GridSystemData3() = default;

private:
	std::vector<Hina::ScalarGrid3Ptr> _scalar_data;
	std::vector<Hina::VectorGrid3Ptr> _vector_data;
	std::vector<Hina::ScalarGrid3Ptr> _advectable_scalar_data;
	std::vector<Hina::VectorGrid3Ptr> _advectable_vector_data;

	FaceCenteredVectorGrid3Ptr _velocity;
	size_t _velocity_index = 0;
};
using GridSystemData3Ptr = std::shared_ptr<GridSystemData3>;
}

#endif //HINAPE_DATA_H
