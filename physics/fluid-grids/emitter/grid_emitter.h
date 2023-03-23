//#ifndef HINAPE_GRID_EMITTER_H
//#define HINAPE_GRID_EMITTER_H
//
//// Copyright (c) 2023 Xayah Hina
//// MPL-2.0 license
//
//#include "common.h"
//
//namespace HinaPE
//{
//class GridEmitter3
//{
//public:
//	virtual void emit(Geom::ScalarGrid3 *grid) = 0;
//};
//
//class VolumeGridEmitter3 : public GridEmitter3
//{
//public:
//	VolumeGridEmitter3();
//
//public:
//	void emit(Geom::ScalarGrid3 *grid) final;
//
//	Geom::ImplicitSurface3Ptr _source_region;
//	real _grid_spacing;
//	const std::function<real(real, const mVector3 &, real)> scalar_mapper = [&](real sdf, const mVector3 &, real old)
//	{
//		real step = Constant::One - Math::smeared_heaviside_sdf(sdf / _grid_spacing); // [-1.5, 1.5] -> [-1, 1]
//		return std::max(old, step); // (max - min) * step + min, max = 1 min = 0
//	};
//	const std::function<real(real, const mVector3 &, const mVector3 &)> vector_mapper = [](real x, const mVector3 &pt, const mVector3 &old)
//	{
//		return x;
//	};
//};
//
//using GridEmitter3Ptr = std::shared_ptr<GridEmitter3>;
//using VolumeGridEmitter3Ptr = std::shared_ptr<VolumeGridEmitter3>;
//} // namespace HinaPE
//
//#endif //HINAPE_GRID_EMITTER_H
