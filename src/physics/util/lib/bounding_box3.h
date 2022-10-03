#ifndef HINAPE_BOUNDING_BOX3_H
#define HINAPE_BOUNDING_BOX3_H

namespace HinaPE
{
template<typename T, size_t N>
class BoundingBox
{
public:
    static_assert(N > 0, "Size of static-sized box should be greater than zero.");

    T l_x, l_y, l_z;
    T u_x, u_y, u_z;

public:
    BoundingBox() = default;
    template<typename Vector3>
    BoundingBox(const Vector3 &lower_corner, const Vector3 &upper_corner) : l_x(lower_corner.x()), l_y(lower_corner.y()), l_z(lower_corner.z()), u_x(upper_corner.x()), u_y(upper_corner.y()), u_z(upper_corner.z()) {}


};
}

#endif //HINAPE_BOUNDING_BOX3_H
