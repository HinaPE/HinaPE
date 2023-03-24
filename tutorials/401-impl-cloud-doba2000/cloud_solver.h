#ifndef HINAPE_CLOUD_SOLVER_H
#define HINAPE_CLOUD_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license
// Implementation of "A simple, efficient method for realistic animation of clouds" by Dobashi, Yoshinori & Kaneda, Kazufumi & Yamashita, Hideo & Okita, Tsuyoshi & Nishita, Tomoyuki.
// https://dl.acm.org/doi/pdf/10.1145/344779.344795

#include "common.h"
#include "backends/objects/object3D.h"
#include "backends/objects/cube.h"
#include "domain/box_domain.h"

class CloudSolver
{
protected:
	void _blur_cloud() const;

public:
	void init();
	void update(real dt) const;
	void reset();

public:
	struct Data;
	std::shared_ptr<Data> _data;
	std::shared_ptr<HinaPE::BoxDomain> _domain;

	struct
	{
		real current_dt = 0.02;
	} Opt;
};

struct CloudSolver::Data : public HinaPE::CopyDisable, public Kasumi::ObjectGrid3D
{
	struct
	{
		HinaPE::Geom::ScalarGridField3 hum; // humidity, 0/1
		HinaPE::Geom::ScalarGridField3 cld; // cloud, 0/1
		HinaPE::Geom::ScalarGridField3 act; // active, 0/1

		HinaPE::Geom::ScalarGridField3 final; // blur of cld
	} Cloud;

	Data(const mVector3 &size, const mSize3 &resolution, const mVector3 &center = mVector3::Zero());
	void reset();
};

#endif //HINAPE_CLOUD_SOLVER_H
