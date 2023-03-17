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
};

using GridEmitter3Ptr = std::shared_ptr<GridEmitter3>;
using VolumeGridEmitter3Ptr = std::shared_ptr<VolumeGridEmitter3>;
} // namespace HinaPE

#endif //HINAPE_GRID_EMITTER_H
