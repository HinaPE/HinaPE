#ifndef HINAPE_CLOTH_H
#define HINAPE_CLOTH_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/objects/object3D.h"

namespace HinaPE
{
class ClothObject : public Kasumi::ObjectMesh3D
{
public:
	// init infos
	std::vector<real> _inv_masses;
	std::vector<mVector3> _init_vertices;
	std::vector<unsigned int> _init_indices;
	std::vector<std::pair<unsigned int, unsigned int>> _init_edges;

	// update infos
	std::vector<mVector3> _positions;
	std::vector<mVector3> _pre_positions;
	std::vector<mVector3> _velocities;
	std::vector<mVector3> _forces;

	ClothObject(real width, real height, size_t rows, size_t cols);

protected:
	void _update_surface() override;

private:
	void build();
	real _width, _height;
	size_t _rows, _cols;
};
using ClothObjectPtr = std::shared_ptr<ClothObject>;
} // namespace HinaPE

#endif //HINAPE_CLOTH_H
