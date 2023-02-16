#ifndef HINAPE_SHAPE_H
#define HINAPE_SHAPE_H

namespace HinaPE::Rigid
{
enum class ShapeType { SPHERE, BOX, NO_SHAPE };
struct ShapeOpt
{
	virtual ~ShapeOpt() = default;
	ShapeType type = ShapeType::NO_SHAPE;
};
struct SphereOpt : public ShapeOpt
{
	SphereOpt() { type = ShapeType::SPHERE; }
	float radius = 1;
};
struct BoxOpt : public ShapeOpt
{
	BoxOpt() { type = ShapeType::BOX; }
	float x = 1, y = 1, z = 1;
};
}

#endif //HINAPE_SHAPE_H
