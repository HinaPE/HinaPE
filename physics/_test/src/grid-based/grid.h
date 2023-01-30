#ifndef HINAPE_GRID_H
#define HINAPE_GRID_H

#include "base/base.h"
#include "field.h"
#include <vector>

namespace Hina
{
// ==================== Grid3 ====================
class Grid3
{
public:
	void resize(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin);

public:
	struct Opt
	{
		Base::Size3 resolution;
		mVector3 origin;
		mVector3 grid_spacing = mVector3(1, 1, 1);
		mBBox3 bounding_box;
	} _opt;
};

// ==================== ScalarGrid3 ====================
class ScalarGrid3 : public Grid3, public ScalarField3
{
public:
	void resize(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, real initial_value);
	virtual inline auto size() const -> Base::Size3 = 0;

private:
	Base::Array3<real> _data;
};
class CellCenteredScalarGrid3 final : public ScalarGrid3
{
public:
	inline auto size() const -> Base::Size3 final { return _opt.resolution; }
};
class VertexCenteredScalarGrid3 final : public ScalarGrid3
{
public:
	inline auto size() const -> Base::Size3 final { return _opt.resolution + Base::Size3(1, 1, 1); }
};

// ==================== VectorGrid3 ====================
class VectorGrid3 : public Grid3, public VectorField3
{
public:
	void resize(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, const mVector3& initial_value);
protected:
	virtual void on_resize(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, const mVector3 &initial_value) = 0;
};
class CollocatedVectorGrid3 : public VectorGrid3
{
protected:
	void on_resize(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, const mVector3 &initial_value) final;

private:
	Base::Array3<mVector3> _data;
};
class CellCenteredVectorGrid3 final : public CollocatedVectorGrid3 {};
class VertexCenteredVectorGrid3 final : public CollocatedVectorGrid3 {};
class FaceCenteredVectorGrid3 final : public VectorGrid3
{
protected:
	void on_resize(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, const mVector3 &initial_value) override;
};

using Grid3Ptr = std::shared_ptr<Grid3>;
using ScalarGrid3Ptr = std::shared_ptr<ScalarGrid3>;
using CellCenteredScalarGrid3Ptr = std::shared_ptr<CellCenteredScalarGrid3>;
using VertexCenteredScalarGrid3Ptr = std::shared_ptr<VertexCenteredScalarGrid3>;
using VectorGrid3Ptr = std::shared_ptr<VectorGrid3>;
using CollocatedVectorGrid3Ptr = std::shared_ptr<CollocatedVectorGrid3>;
using CellCenteredVectorGrid3Ptr = std::shared_ptr<CellCenteredVectorGrid3>;
using VertexCenteredVectorGrid3Ptr = std::shared_ptr<VertexCenteredVectorGrid3>;
using FaceCenteredVectorGrid3Ptr = std::shared_ptr<FaceCenteredVectorGrid3>;
}

#endif //HINAPE_GRID_H
