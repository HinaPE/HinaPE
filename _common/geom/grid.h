#ifndef HINAPE_GRID_H
#define HINAPE_GRID_H

#include "field.h"
#include "math/array.h"
#include "math/size.h"
#include "geom/bbox.h"

namespace HinaPE::Geom
{
// ============================== Grid3 ==============================
class Grid3
{
public:
	using DataPositionFunc = std::function<mVector3(size_t, size_t, size_t)>;
	auto cell_center_position() const -> DataPositionFunc;
	void for_each_cell_index(const std::function<void(size_t, size_t, size_t)> &func) const { for (size_t k = 0; k < _opt.resolution.z; ++k) for (size_t j = 0; j < _opt.resolution.y; ++j) for (size_t i = 0; i < _opt.resolution.x; ++i) func(i, j, k); }
	void parallel_for_each_cell_index(const std::function<void(size_t, size_t, size_t)> &func) const { Util::parallelFor((size_t) 0, _opt.resolution.x, (size_t) 0, _opt.resolution.y, (size_t) 0, _opt.resolution.z, [&func](size_t i, size_t j, size_t k) { func(i, j, k); }); }

public:
	struct Opt
	{
		mSize3 resolution;
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
	void for_each_data_point_index(const std::function<void(size_t, size_t, size_t)> &func) const { _data.for_each_index(func); }
	void parallel_for_each_data_point_index(const std::function<void(size_t, size_t, size_t)> &func) const { _data.parallel_for_each_index(func); }
	void fill(real value, Util::ExecutionPolicy policy = Util::ExecutionPolicy::Parallel) { Util::parallelFor((size_t) 0, _data.size().x, (size_t) 0, _data.size().y, (size_t) 0, _data.size().z, [&](size_t i, size_t j, size_t k) { _data(i, j, k) = value; }, policy); }
	void fill(const std::function<real(const mVector3 &)> &func, Util::ExecutionPolicy policy = Util::ExecutionPolicy::Parallel)
	{
		auto pos = data_position();
		Util::parallelFor((size_t) 0, _data.size().x, (size_t) 0, _data.size().y, (size_t) 0, _data.size().z, [&](size_t i, size_t j, size_t k) { _data(i, j, k) = func(pos(i, j, k)); }, policy);
	}

public: // implement ScalarField3
	inline auto sample(const mVector3 &x) const -> real final { return _sampler(x); }
	inline auto sampler() const -> std::function<real(const mVector3 &)> final { return _sampler; }
	inline auto gradient(const mVector3 &x) const -> mVector3 final
	{
		std::array<mVector3u, 8> indices{};
		std::array<real, 8> weights{};
		_linear_sampler.get_coordinate_and_weights(x, &indices, &weights);

		mVector3 res = mVector3::Zero();
		for (size_t i = 0; i < 8; ++i)
			res += weights[i] * gradient_at_data_point(indices[i].x(), indices[i].y(), indices[i].z());

		return res;
	}
	inline auto laplacian(const mVector3 &x) const -> real final
	{
		std::array<mVector3u, 8> indices{};
		std::array<real, 8> weights{};
		_linear_sampler.get_coordinate_and_weights(x, &indices, &weights);

		real result = Constant::Zero;
		for (size_t i = 0; i < 8; ++i)
			result += weights[i] * laplacian_at_data_point(indices[i].x(), indices[i].y(), indices[i].z());

		return result;
	}

public: // math
	inline auto gradient_at_data_point(size_t i, size_t j, size_t k) const -> mVector3 { return gradient3(_data, _opt.grid_spacing, i, j, k); } // TODO: remove
	inline auto laplacian_at_data_point(size_t i, size_t j, size_t k) const -> real { return laplacian3(_data, _opt.grid_spacing, i, j, k); } // TODO: remove

	using DataPositionFunc = std::function<mVector3(size_t, size_t, size_t)>;
	auto data_position() const -> DataPositionFunc { return [this](size_t i, size_t j, size_t k) -> mVector3 { return _opt.origin + _opt.grid_spacing * mVector3({i, j, k}); }; }
	virtual inline auto data_size() const -> mSize3 = 0; /// not necessarily equal to _opt.resolution
	virtual inline auto data_origin() const -> mVector3 = 0; /// not necessarily equal to _opt.origin
	virtual auto clone() const -> std::shared_ptr<ScalarGrid3> = 0;
	void resize(const mSize3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, real initial_value);
	void clear() { resize(mSize3(0, 0, 0), _opt.grid_spacing, _opt.origin, Constant::Zero); }

public: // constructors & destructor & assignment operators
	ScalarGrid3() : _linear_sampler(_data) { _sampler = _linear_sampler.functor(); }
	inline auto operator()(size_t i, size_t j, size_t k) -> real & { return _data(i, j, k); }
	inline auto operator()(size_t i, size_t j, size_t k) const -> const real & { return _data(i, j, k); }

private:
	Math::Array3<real> _data;
	Math::LinearArray3Sampler<real, real> _linear_sampler;
	std::function<real(const mVector3 &)> _sampler;
};
class CellCenteredScalarGrid3 final : public ScalarGrid3
{
public:
	inline auto data_size() const -> mSize3 final { return _opt.resolution; }
	inline auto data_origin() const -> mVector3 final { return _opt.origin + Constant::Half * _opt.grid_spacing; }
	auto clone() const -> std::shared_ptr<ScalarGrid3> final { new CellCenteredScalarGrid3(*this), [](CellCenteredScalarGrid3 *obj) { delete obj; }; }
};
class VertexCenteredScalarGrid3 final : public ScalarGrid3
{
public:
	inline auto data_size() const -> mSize3 final { return _opt.resolution + mSize3(1, 1, 1); }
	inline auto data_origin() const -> mVector3 final { return _opt.origin; }
	auto clone() const -> std::shared_ptr<ScalarGrid3> final { new VertexCenteredScalarGrid3(*this), [](VertexCenteredScalarGrid3 *obj) { delete obj; }; }
};
// ============================== ScalarGrid3 ==============================



// ============================== VectorGrid3 ==============================
class VectorGrid3 : public Grid3, public VectorField3
{
public:
	virtual void fill(const mVector3 &value, Util::ExecutionPolicy policy) = 0;
	virtual void fill(const std::function<mVector3(const mVector3 &)> &func, Util::ExecutionPolicy policy) = 0;
	virtual auto clone() -> std::shared_ptr<VectorGrid3> = 0;
public: // implement VectorField3
	/* NOT IMPLEMENTED */ auto sample(const mVector3 &x) const -> mVector3 final { return mVector3::Zero(); }
	/* NOT IMPLEMENTED */ auto sampler() const -> std::function<mVector3(const mVector3 &)> final { return [this](const mVector3 &x) { return sample(x); }; }
	/* NOT IMPLEMENTED */ auto divergence(const mVector3 &x) const -> real final { return Constant::Zero; }
	/* NOT IMPLEMENTED */ auto curl(const mVector3 &x) const -> mVector3 final { return mVector3::Zero(); }
public:
	void resize(const mSize3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, const mVector3 &initial_value);
protected:
	virtual void on_resize(const mSize3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, const mVector3 &initial_value) = 0;
};
class CollocatedVectorGrid3 : public VectorGrid3
{
protected:
	void on_resize(const mSize3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, const mVector3 &initial_value) final;

private:
	Math::Array3<mVector3> _data;
};
class CellCenteredVectorGrid3 final : public CollocatedVectorGrid3
{
public:
	void fill(const mVector3 &value, Util::ExecutionPolicy policy) override
	{
	}
	void fill(const std::function<mVector3(const mVector3 &)> &func, Util::ExecutionPolicy policy) override
	{
	}
	auto clone() -> std::shared_ptr<VectorGrid3> override
	{
		return {new CellCenteredVectorGrid3(*this), [](CellCenteredVectorGrid3 *obj) { delete obj; }};
	}
};
class VertexCenteredVectorGrid3 final : public CollocatedVectorGrid3
{
public:
	void fill(const mVector3 &value, Util::ExecutionPolicy policy) override
	{
	}
	void fill(const std::function<mVector3(const mVector3 &)> &func, Util::ExecutionPolicy policy) override
	{
	}
	auto clone() -> std::shared_ptr<VectorGrid3> override
	{
		return {new VertexCenteredVectorGrid3(*this), [](VertexCenteredVectorGrid3 *obj) { delete obj; }};
	}
};
class FaceCenteredVectorGrid3 final : public VectorGrid3
{
public:
	inline auto u(size_t i, size_t j, size_t k) -> real & { return _u_data(i, j, k); }
	inline auto v(size_t i, size_t j, size_t k) -> real & { return _v_data(i, j, k); }
	inline auto w(size_t i, size_t j, size_t k) -> real & { return _w_data(i, j, k); }
	inline auto u(size_t i, size_t j, size_t k) const -> const real & { return _u_data(i, j, k); }
	inline auto v(size_t i, size_t j, size_t k) const -> const real & { return _v_data(i, j, k); }
	inline auto w(size_t i, size_t j, size_t k) const -> const real & { return _w_data(i, j, k); }

	void for_each_u_index(const std::function<void(size_t, size_t, size_t)> &func) const { _u_data.for_each_index(func); }
	void for_each_v_index(const std::function<void(size_t, size_t, size_t)> &func) const { _v_data.for_each_index(func); }
	void for_each_w_index(const std::function<void(size_t, size_t, size_t)> &func) const { _w_data.for_each_index(func); }
	void parallel_for_each_u_index(const std::function<void(size_t, size_t, size_t)> &func) const { _u_data.parallel_for_each_index(func); }
	void parallel_for_each_v_index(const std::function<void(size_t, size_t, size_t)> &func) const { _v_data.parallel_for_each_index(func); }
	void parallel_for_each_w_index(const std::function<void(size_t, size_t, size_t)> &func) const { _w_data.parallel_for_each_index(func); }

	void fill(const mVector3 &value, Util::ExecutionPolicy policy) override {}
	void fill(const std::function<mVector3(const mVector3 &)> &func, Util::ExecutionPolicy policy) override {}
	auto clone() -> std::shared_ptr<VectorGrid3> override { return {new FaceCenteredVectorGrid3(*this), [](FaceCenteredVectorGrid3 *obj) { delete obj; }}; }
public: // math
	auto value_at_cell_center(size_t i, size_t j, size_t k) const -> mVector3;
	auto divergence_at_cell_center(size_t i, size_t j, size_t k) const -> real;
	auto curl_at_cell_center(size_t i, size_t j, size_t k) const -> mVector3;

public:
	explicit FaceCenteredVectorGrid3() : _u_sampler(_u_data), _v_sampler(_v_data), _w_sampler(_w_data) { _sampler = [&](const mVector3 &x) -> mVector3 { return {_u_sampler(x), _v_sampler(x), _w_sampler(x)}; }; }

protected:
	void on_resize(const mSize3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin, const mVector3 &initial_value) final;

private:
	Math::Array3<real> _u_data;
	Math::Array3<real> _v_data;
	Math::Array3<real> _w_data;
	mVector3 _u_origin;
	mVector3 _v_origin;
	mVector3 _w_origin;
	Math::LinearArray3Sampler<real, real> _u_sampler;
	Math::LinearArray3Sampler<real, real> _v_sampler;
	Math::LinearArray3Sampler<real, real> _w_sampler;
	std::function<mVector3(const mVector3 &)> _sampler;
};
// ============================== VectorGrid3 ==============================



//@formatter:off
using Grid3Ptr                     = std::shared_ptr<Grid3>;
using ScalarGrid3Ptr               = std::shared_ptr<ScalarGrid3>;
using CellCenteredScalarGrid3Ptr   = std::shared_ptr<CellCenteredScalarGrid3>;
using VertexCenteredScalarGrid3Ptr = std::shared_ptr<VertexCenteredScalarGrid3>;
using VectorGrid3Ptr               = std::shared_ptr<VectorGrid3>;
using CollocatedVectorGrid3Ptr     = std::shared_ptr<CollocatedVectorGrid3>;
using CellCenteredVectorGrid3Ptr   = std::shared_ptr<CellCenteredVectorGrid3>;
using VertexCenteredVectorGrid3Ptr = std::shared_ptr<VertexCenteredVectorGrid3>;
using FaceCenteredVectorGrid3Ptr   = std::shared_ptr<FaceCenteredVectorGrid3>;
//@formatter:on
}

#endif //HINAPE_GRID_H
