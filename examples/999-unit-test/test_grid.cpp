#include "test_grid.h"

void setup_lines(const HinaPE::ScalarGrid3Ptr &grid, Kasumi::LinesPtr &lines, const mVector3 &color = Color::GRAY)
{
	lines->clear();
	const auto &res = grid->resolution();
	const auto &origin = grid->origin();
	const auto &spacing = grid->gridSpacing();
	for (size_t i = 0; i < res.x; ++i)
	{
		for (size_t j = 0; j < res.y; ++j)
		{
			for (size_t k = 0; k < res.z; ++k)
			{
				auto as_m_vector = [&](const HinaPE::Vector3D &src) -> mVector3 { return {src.x, src.y, src.z}; };

				auto o = origin + spacing * HinaPE::Vector3D(i, j, k);
				auto p1 = as_m_vector(o);
				auto p2 = as_m_vector(o + spacing * HinaPE::Vector3D(1, 0, 0));
				auto p3 = as_m_vector(o + spacing * HinaPE::Vector3D(0, 1, 0));
				auto p4 = as_m_vector(o + spacing * HinaPE::Vector3D(0, 0, 1));
				auto p5 = as_m_vector(o + spacing * HinaPE::Vector3D(1, 1, 0));
				auto p6 = as_m_vector(o + spacing * HinaPE::Vector3D(1, 0, 1));
				auto p7 = as_m_vector(o + spacing * HinaPE::Vector3D(0, 1, 1));
				auto p8 = as_m_vector(o + spacing * HinaPE::Vector3D(1, 1, 1));

				lines->add(p1, p2, color);
				lines->add(p1, p3, color);
				lines->add(p1, p4, color);
				lines->add(p2, p5, color);
				lines->add(p2, p6, color);
				lines->add(p3, p5, color);
				lines->add(p3, p7, color);
				lines->add(p4, p6, color);
				lines->add(p4, p7, color);
				lines->add(p5, p8, color);
				lines->add(p6, p8, color);
				lines->add(p7, p8, color);
			}
		}
	}
}

void TestGrid::prepare()
{
	// sim a grid
	auto grid = HinaPE::CellCenteredScalarGrid3::builder()
			.withOrigin({-5, -5, -5})
			.withGridSpacing({2, 2, 2})
			.withInitialValue(0)
			.withResolution({5, 5, 5})
			.makeShared();
	auto lines = std::make_shared<Kasumi::Lines>();
	lines->_opt._opacity = 0.3f;
	setup_lines(grid, lines, Color::GRAY);
	auto model = std::make_shared<Kasumi::Model>(lines);
	_grid_obj = _scene->get_object(_scene->add_object(model));
}
