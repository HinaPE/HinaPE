// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license
// Implementation of "A simple, efficient method for realistic animation of clouds" by Dobashi, Yoshinori & Kaneda, Kazufumi & Yamashita, Hideo & Okita, Tsuyoshi & Nishita, Tomoyuki.
// https://dl.acm.org/doi/pdf/10.1145/344779.344795

#include "common.h"
#ifdef HINAPE_OPENVDB
#include <openvdb/openvdb.h>
#include <openvdb/tools/LevelSetSphere.h>
#endif

auto main() -> int
{
#ifdef HINAPE_OPENVDB
	openvdb::initialize();
	// Create a FloatGrid and populate it with a narrow-band
	// signed distance field of a sphere.
	openvdb::FloatGrid::Ptr grid =
			openvdb::tools::createLevelSetSphere<openvdb::FloatGrid>(
					/*radius=*/50.0, /*center=*/openvdb::Vec3f(1.5, 2, 3),
					/*voxel size=*/0.5, /*width=*/4.0);
	// Associate some metadata with the grid.
	grid->insertMeta("radius", openvdb::FloatMetadata(50.0));
	// Name the grid "LevelSetSphere".
	grid->setName("LevelSetSphere");
	// Create a VDB file object and write out the grid.
	openvdb::io::File("mygrids.vdb").write({grid});
#endif
	return 0;
}