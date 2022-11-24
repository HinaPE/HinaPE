#include "cloth_solver.h"
#include "parallel.h"

#include <algorithm>

HinaPE::Cloth::ClothSolver::ClothSolver(const HinaPE::Cloth::ClothSolver::Opt &opt) : _opt(opt) { setup(); }
void HinaPE::Cloth::ClothSolver::step(HinaPE::Cloth::real dt) {}
void HinaPE::Cloth::ClothSolver::setup()
{
    _cloth_data = nullptr;
    _cloth_data = std::make_shared<ClothData>();

    // init geometry data
    auto &_vertices = _cloth_data->_init_vertices;
    auto &_indices = _cloth_data->_init_indices;
    _vertices.resize(_opt.cols * _opt.rows);
    _indices.resize((_opt.cols - 1) * (_opt.rows - 1) * 6);
    for (int j = 0; j < _opt.rows; ++j)
        for (int i = 0; i < _opt.cols; ++i)
            _vertices[j * _opt.cols + i] = mVector3(_opt.width * (i / (_opt.cols - 1.f)), _opt.height * (j / (_opt.rows - 1.f)), 0.f);
    for (int j = 0; j < _opt.rows - 1; ++j)
    {
        for (int i = 0; i < _opt.cols - 1; ++i)
        {
            _indices[(j * (_opt.cols - 1) + i) * 6 + 0] = j * _opt.cols + i;
            _indices[(j * (_opt.cols - 1) + i) * 6 + 1] = j * _opt.cols + i + 1;
            _indices[(j * (_opt.cols - 1) + i) * 6 + 2] = (j + 1) * _opt.cols + i;
            _indices[(j * (_opt.cols - 1) + i) * 6 + 3] = (j + 1) * _opt.cols + i;
            _indices[(j * (_opt.cols - 1) + i) * 6 + 4] = j * _opt.cols + i + 1;
            _indices[(j * (_opt.cols - 1) + i) * 6 + 5] = (j + 1) * _opt.cols + i + 1;
        }
    }

    // init physics data
    auto &_positions = _cloth_data->_positions;
    auto &_velocities = _cloth_data->_velocities;
    auto &_forces = _cloth_data->_forces;
    auto &_masses = _cloth_data->_masses;

    _positions.resize(_vertices.size());
    _velocities.resize(_vertices.size());
    _forces.resize(_vertices.size());
    _masses.resize(_vertices.size());
    std::copy(_vertices.begin(), _vertices.end(), _positions.begin());
    std::fill(_vertices.begin(), _vertices.end(), mVector3(0.f, 0.f, 0.f));
    std::fill(_forces.begin(), _forces.end(), mVector3(0.f, 0.f, 0.f));
    std::fill(_masses.begin(), _masses.end(), 1e-3);
}
auto HinaPE::Cloth::ClothSolver::vertices() -> std::vector<HinaPE::Cloth::mVector3> & { return _cloth_data->_positions; }
auto HinaPE::Cloth::ClothSolver::indices() -> std::vector<unsigned int> & { return _cloth_data->_init_indices; }
