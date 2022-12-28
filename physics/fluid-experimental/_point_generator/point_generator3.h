#ifndef HINAPE_POINT_GENERATOR3_H
#define HINAPE_POINT_GENERATOR3_H

#include "math_api.h"
#include "array/array1.h"

#include <memory>

namespace HinaPE
{
class PointGenerator3
{
public:
	virtual void forEachPoint(const mBBox &bounding_box, real spacing, const std::function<bool(const mVector3 &)> &callback) const = 0;
	void generate(const mBBox &boundingBox, real spacing, Array1<mVector3> *points) const;
};
using PointGenerator3Ptr = std::shared_ptr<PointGenerator3>;

class BccLatticePointGenerator3 final : public PointGenerator3
{
public:
	//!
	//! \brief Body-centered lattice points generator.
	//!
	//! \see http://en.wikipedia.org/wiki/Cubic_crystal_system
	//!      http://mathworld.wolfram.com/CubicClosePacking.html
	//!
	//! \brief Invokes \p callback function for each BCC-lattice points inside
	//! \p boundingBox.
	//!
	//! This function iterates every BCC-lattice points inside \p boundingBox
	//! where \p spacing is the size of the unit cell of BCC structure.
	//!
	void forEachPoint(const mBBox &bounding_box, real spacing, const std::function<bool(const mVector3 &)> &callback) const override;
};
using BccLatticePointGenerator3Ptr = std::shared_ptr<BccLatticePointGenerator3>;
}

#endif //HINAPE_POINT_GENERATOR3_H
