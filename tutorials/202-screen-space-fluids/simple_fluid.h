#ifndef HINAPE_SIMPLE_FLUID_H
#define HINAPE_SIMPLE_FLUID_H

#include "common/common.h"

namespace HinaPE
{
class SimpleFluid
{
public:
	SimpleFluid();

private:
	struct Data;
	std::shared_ptr<Data> _data;
};

struct SimpleFluid::Data
{
	struct
	{
		std::vector<mVector3> positions;
	} Fluid;

	Data();
};
}

#endif //HINAPE_SIMPLE_FLUID_H
