#ifndef HINAPE_EMITTER_H
#define HINAPE_EMITTER_H

#include "base/base.h"

namespace Hina
{
class GridEmitter3
{
public:

public:
	struct Opt
	{
		bool is_enabled = false;
		bool is_one_shot = true;
	} _opt;

protected:
	virtual void on_update(real current_time, real time_interval) = 0;
};
using GridEmitter3Ptr = std::shared_ptr<GridEmitter3>;

class VolumeGridEmitter3 final : public GridEmitter3
{
protected:
	void on_update(real current_time, real time_interval) final;
};
}

#endif //HINAPE_EMITTER_H
