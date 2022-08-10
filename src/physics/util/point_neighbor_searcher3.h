#ifndef HINAPE_POINT_NEIGHBOR_SEARCHER3_H
#define HINAPE_POINT_NEIGHBOR_SEARCHER3_H

#include "../lib/basic.h"
#include "../lib/array_accessor1.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace HinaPE
{
class PointNeighborSearcher3
{
    using ForEachNearbyPointFunc = std::function<void(size_t, const Vector3D &)>;

public:
    PointNeighborSearcher3() = default;
    virtual ~PointNeighborSearcher3() = default;

    virtual auto build(const ConstArrayAccessor1<Vector3D> &points) -> void = 0;
    virtual void foreach_nearby_point(const Vector3D &origin, double radius, const ForEachNearbyPointFunc &callback) const = 0;
    [[nodiscard]] virtual auto typeName() const -> std::string = 0;
    [[nodiscard]] virtual auto has_nearby_point(const Vector3D &origin, double radius) const -> bool = 0;
    [[nodiscard]] virtual auto clone() const -> std::shared_ptr<PointNeighborSearcher3> = 0;
};
using PointNeighborSearcher3Ptr = std::shared_ptr<PointNeighborSearcher3>;

class PointNeighborSearcherBuilder3
{
public:
    [[nodiscard]] virtual PointNeighborSearcher3Ptr buildPointNeighborSearcher() const = 0;
};
}

#endif //HINAPE_POINT_NEIGHBOR_SEARCHER3_H
