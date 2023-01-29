#ifndef HINAPE_GRID_H
#define HINAPE_GRID_H

#include "base/base.h"
#include <vector>

namespace HinaPE
{
class Grid3
{
public:
	struct Opt
	{
		Base::Size3 resolution;
		Base::mVector3 origin;
		Base::mVector3 grid_spacing = Base::mVector3(1, 1, 1);
		Base::mBBox3 bounding_box;
	} _opt;
};

class ScalarGrid3 : public Grid3
{
public:
	struct Opt : public Grid3::Opt
	{
	} _opt;

private:
	std::vector<Base::real> _data;
};

class VectorGrid3 : public Grid3
{
public:
	struct Opt : public Grid3::Opt
	{
	} _opt;
};

class CollocatedVectorGrid3 : public VectorGrid3
{
private:
	Base::Array3<Base::mVector3> _data;
};

class CellCenteredVectorGrid3 final : public CollocatedVectorGrid3 {};

class VertexCenteredVectorGrid3 final : public CollocatedVectorGrid3 {};
}

#endif //HINAPE_GRID_H
