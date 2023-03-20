// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license
// Implementation of "A simple, efficient method for realistic animation of clouds" by Dobashi, Yoshinori & Kaneda, Kazufumi & Yamashita, Hideo & Okita, Tsuyoshi & Nishita, Tomoyuki.
// https://dl.acm.org/doi/pdf/10.1145/344779.344795

#include "common.h"

struct CloudGrid3D
{
	void resize(int x, int y, int z)
	{
		_x = x;
		_y = y;
		_z = z;
		_data.resize(x * y * z);
	}

	auto get(int x, int y, int z) const -> bool
	{
		return _data[x + y * _x + z * _x * _y];
	}

	void set(int x, int y, int z, bool value)
	{
		_data[x + y * _x + z * _x * _y] = value;
	}

	int _x;
	int _y;
	int _z;
	std::vector<bool> _data; // Important: take care of std::vector<bool>
};

auto main() -> int
{
	return 0;
}