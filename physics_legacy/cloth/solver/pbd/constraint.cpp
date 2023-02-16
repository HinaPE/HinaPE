#include "constraint.h"
HinaPE::Cloth::Constraint::Constraint(const HinaPE::Cloth::Constraint::Opt &opt) : _opt(opt) {}
HinaPE::Cloth::DistanceConstraint::DistanceConstraint(const HinaPE::Cloth::DistanceConstraint::Opt &opt, std::vector<mVector3> &vertices, std::vector<real> &inv_masses, const std::vector<std::pair<unsigned int, unsigned int>> &edges) : Constraint(opt), _opt(opt),
                                                                                                                                                                                                                                            _vertices(vertices),
                                                                                                                                                                                                                                            _inv_masses(inv_masses)
{
    for (auto &edge: edges)
        _pairs.emplace_back(std::make_tuple(edge.first, edge.second, (vertices[edge.first] - vertices[edge.second]).length()));
}
void HinaPE::Cloth::DistanceConstraint::solve()
{
    if (_vertices.empty())
        return;

    for (auto &pair: _pairs)
    {
        auto p1_id = std::get<0>(pair);
        auto p2_id = std::get<1>(pair);
        auto rest_length = std::get<2>(pair);

        auto &p1 = _vertices[p1_id];
        auto &p2 = _vertices[p2_id];

        auto diff = p2 - p1;
        auto current_length = diff.length();
        auto C = current_length - rest_length;
        auto dC = diff / current_length;

        real w1, w2;
        if (_inv_masses[p1_id] == 0.f && _inv_masses[p2_id] == 0.f)
            continue;
        else if (_inv_masses[p1_id] == 0.f)
            w1 = 0.f, w2 = 1.f;
        else if (_inv_masses[p2_id] == 0.f)
            w1 = 1.f, w2 = 0.f;
        else
        {
            real m1 = 1.f / _inv_masses[p1_id];
            real m2 = 1.f / _inv_masses[p2_id];
            w1 = m1 / (m1 + m2);
            w2 = m2 / (m1 + m2);
        }

        p1 += _opt.stiffness * dC * w1 * C;
        p2 -= _opt.stiffness * dC * w2 * C;
    }
}
