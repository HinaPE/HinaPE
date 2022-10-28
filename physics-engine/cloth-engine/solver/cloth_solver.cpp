#include "cloth_solver.h"

auto HinaPE::ClothEngine::ClothSolver::builder() -> HinaPE::ClothEngine::ClothSolver::Builder { return {}; }
HinaPE::ClothEngine::ClothSolver::ClothSolver() = default;
void HinaPE::ClothEngine::ClothSolver::on_begin_advance_time_step(double time_interval_in_seconds) {}
void HinaPE::ClothEngine::ClothSolver::on_end_advance_time_step(double time_interval_in_seconds) {}
auto HinaPE::ClothEngine::ClothSolver::Builder::build() const -> HinaPE::ClothEngine::ClothSolver { return {}; }
auto HinaPE::ClothEngine::ClothSolver::Builder::make_shared() const -> HinaPE::ClothEngine::ClothSolverPtr { return {new ClothSolver(), [](ClothSolver *obj) { delete obj; }}; }

void HinaPE::ClothEngine::ClothSolver::onInitialize() { PhysicsAnimation::onInitialize(); }
void HinaPE::ClothEngine::ClothSolver::onAdvanceTimeStep(double timeIntervalInSeconds)
{
    on_begin_advance_time_step(timeIntervalInSeconds);

    on_end_advance_time_step(timeIntervalInSeconds);
}

//void HinaPE::ClothEngine::ClothSolver::Builder::create_grid(const HinaPE::Vector3D &center_point, const HinaPE::Vector3D &orientation, double width, double height, int rows, int cols)
//{
//    //    std::vector<Vector3> _nodes;
//    //    std::vector<int> _indices;
//    //
//    //    _nodes.resize(rows * cols);
//    //    HinaPE::parallelFor(0, rows, 0, cols, [&](size_t i, size_t j)
//    //    {
//    //        auto index = i * cols + j;
//    //        real elem1 = as_real(j) / (as_real(cols - 1)) * width - width / 2.0f;
//    //        real elem2 = as_real(i) / (as_real(rows - 1)) * height - height / 2.0f;
//    //
//    //        Vector3 v1(0, 0, as_real(1));
//    //        Vector3 v2 = orientation.normalized();
//    //        auto axis = v1.cross(v2);
//    //        auto radians = std::acos(v1.dot(v2));
//    //        Quaternion q(axis, radians);
//    //        Vector3 res = q.mul(Vector3(elem1, elem2, 0));
//    //
//    //        _nodes[index] = res;
//    //    });
//}