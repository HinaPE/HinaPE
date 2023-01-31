#ifndef HINAPE_GRID_H
#define HINAPE_GRID_H

#include "base/base.h"
#include "field.h"
#include <vector>

namespace Hina
{
// ============================== Grid3 ==============================
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
// ============================== Grid3 ==============================



// ============================== ScalarGrid3 ==============================
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
// ============================== ScalarGrid3 ==============================



// ============================== VectorGrid3 ==============================
class VectorGrid3 : public Grid3, public VectorField3
{
public:
	void resize(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, const mVector3 &initial_value);
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
public:
	inline auto u(size_t i, size_t j, size_t k) -> real & { return _u_data(i, j, k); }
	inline auto v(size_t i, size_t j, size_t k) -> real & { return _v_data(i, j, k); }
	inline auto w(size_t i, size_t j, size_t k) -> real & { return _w_data(i, j, k); }
	inline auto u(size_t i, size_t j, size_t k) const -> const real & { return _u_data(i, j, k); }
	inline auto v(size_t i, size_t j, size_t k) const -> const real & { return _v_data(i, j, k); }
	inline auto w(size_t i, size_t j, size_t k) const -> const real & { return _w_data(i, j, k); }

	inline void for_each_u_index(const std::function<void(size_t, size_t, size_t)> &func) const {}
	inline void for_each_v_index(const std::function<void(size_t, size_t, size_t)> &func) const {}
	inline void for_each_w_index(const std::function<void(size_t, size_t, size_t)> &func) const {}
	inline void parallel_for_each_u_index(const std::function<void(size_t, size_t, size_t)> &func) const {}
	inline void parallel_for_each_v_index(const std::function<void(size_t, size_t, size_t)> &func) const {}
	inline void parallel_for_each_w_index(const std::function<void(size_t, size_t, size_t)> &func) const {}

public:
	explicit FaceCenteredVectorGrid3() : _u_sampler(_u_data), _v_sampler(_v_data), _w_sampler(_w_data) {}

protected:
	void on_resize(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, const mVector3 &initial_value) override;

private:
	Base::Array3<real> _u_data;
	Base::Array3<real> _v_data;
	Base::Array3<real> _w_data;
	mVector3 _u_origin;
	mVector3 _v_origin;
	mVector3 _w_origin;
	Base::LinearArray3Sampler<real, real> _u_sampler;
	Base::LinearArray3Sampler<real, real> _v_sampler;
	Base::LinearArray3Sampler<real, real> _w_sampler;
	std::function<mVector3(const mVector3 &)> _sampler;
};
// ============================== VectorGrid3 ==============================



// ============================== Shared pointers ==============================
using Grid3Ptr = std::shared_ptr<Grid3>;
using ScalarGrid3Ptr = std::shared_ptr<ScalarGrid3>;
using CellCenteredScalarGrid3Ptr = std::shared_ptr<CellCenteredScalarGrid3>;
using VertexCenteredScalarGrid3Ptr = std::shared_ptr<VertexCenteredScalarGrid3>;
using VectorGrid3Ptr = std::shared_ptr<VectorGrid3>;
using CollocatedVectorGrid3Ptr = std::shared_ptr<CollocatedVectorGrid3>;
using CellCenteredVectorGrid3Ptr = std::shared_ptr<CellCenteredVectorGrid3>;
using VertexCenteredVectorGrid3Ptr = std::shared_ptr<VertexCenteredVectorGrid3>;
using FaceCenteredVectorGrid3Ptr = std::shared_ptr<FaceCenteredVectorGrid3>;
// ============================== Shared pointers ==============================
}

#endif //HINAPE_GRID_H
