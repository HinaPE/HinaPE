#ifndef HINAPE_CLOUD_SOLVER_H
#define HINAPE_CLOUD_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license
// Implementation of "A simple, efficient method for realistic animation of clouds" by Dobashi, Yoshinori & Kaneda, Kazufumi & Yamashita, Hideo & Okita, Tsuyoshi & Nishita, Tomoyuki.
// https://dl.acm.org/doi/pdf/10.1145/344779.344795

#include "common.h"
#include "backends/objects/object3D.h"
#include "backends/objects/cube.h"

struct BoxDomain : public Kasumi::CubeObject
{
public:
	BoxDomain()
	{
		NAME = "Domain";
		_switch_surface();
		_switch_bbox();
		flip_normal(); // for inner collision
		set_color(HinaPE::Color::BLUE);
	}
};

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
	std::shared_ptr<BoxDomain> _domain;

	struct
	{
		real current_dt = 0.02;
	} Opt;
};

struct CloudSolver::Data : public HinaPE::CopyDisable, public Kasumi::ObjectGrid3D
{
	struct
	{
		HinaPE::Geom::DataGrid3<real> hum; // humidity, 0/1
		HinaPE::Geom::DataGrid3<real> cld; // cloud, 0/1
		HinaPE::Geom::DataGrid3<real> act; // active, 0/1

		HinaPE::Geom::DataGrid3<real> final; // blur of cld
	} Cloud;

	Data(const mVector3 &size, const mSize3 &resolution, const mVector3 &center = mVector3::Zero());
	void reset();
};

#endif //HINAPE_CLOUD_SOLVER_H
