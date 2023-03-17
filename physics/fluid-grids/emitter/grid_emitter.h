#ifndef HINAPE_GRID_EMITTER_H
#define HINAPE_GRID_EMITTER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "common.h"

namespace HinaPE
{
class GridEmitter3
{
public:
	virtual void emit() = 0;
};

class VolumeGridEmitter3 : public GridEmitter3
{
public:
	void emit() final;

	Geom::ImplicitSurface3Ptr _source_region = std::make_shared<Geom::SurfaceToImplicit3>(std::make_shared<Geom::Sphere3>());
	const std::function<real(real, const mVector3 &, real)> scalar_mapper = [](real sdf, const mVector3 &, real old)
	{
		const real grid_spacing = 0.1;
		real step = Constant::One - Math::smeared_heaviside_sdf(sdf / grid_spacing);
		return std::max(old, step); // (max - min) * step + min, max = 1 min = 0
	};
	const std::function<real(real, const mVector3 &, const mVector3 &)> vector_mapper = [](real x, const mVector3 &pt, const mVector3 &old)
	{
		return x;
	};
};

using GridEmitter3Ptr = std::shared_ptr<GridEmitter3>;
using VolumeGridEmitter3Ptr = std::shared_ptr<VolumeGridEmitter3>;
} // namespace HinaPE

#endif //HINAPE_GRID_EMITTER_H
