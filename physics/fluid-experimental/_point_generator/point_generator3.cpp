#include "point_generator3.h"

void HinaPE::PointGenerator3::generate(const mBBox &boundingBox, real spacing, HinaPE::Array1<mVector3> *points) const
{
	forEachPoint(boundingBox, spacing, [&points](const mVector3 &point)
	{
		points->append(point);
		return true;
	});
}
void HinaPE::BccLatticePointGenerator::forEachPoint(const mBBox &bounding_box, real spacing, const std::function<bool(const mVector3 &)> &callback) const
{
	real half_spacing = spacing / static_cast<real>(2.0);
	real box_width = bounding_box.width();
	real box_height = bounding_box.height();
	real box_depth = bounding_box.depth();

	mVector3 position;
	bool has_offset = false;
	bool should_quit = false;
	for (int k = 0; static_cast<real>(k) * half_spacing <= box_depth && !should_quit; ++k)
	{
		position.z = static_cast<real>(k) * half_spacing + bounding_box.lowerCorner.z;

		real offset = (has_offset) ? half_spacing : static_cast<real>(0.0);

		for (int j = 0; static_cast<real>(j) * spacing + offset <= box_height && !should_quit; ++j)
		{
			position.y = static_cast<real>(j) * spacing + offset + bounding_box.lowerCorner.y;

			for (int i = 0; static_cast<real>(i) * spacing + offset <= box_width; ++i)
			{
				position.x = static_cast<real>(i) * spacing + offset + bounding_box.lowerCorner.x;
				if (!callback(position))
				{
					should_quit = true;
					break;
				}
			}
		}

		has_offset = !has_offset;
	}
}
