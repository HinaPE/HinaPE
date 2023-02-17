#include "solver.h"

HinaPE::SPHSolver::SPHSolver()
{
	_data = std::make_shared<Data>();
	_data->_rebuild_();
	_kernel = std::make_shared<StdKernel>();
	dynamic_cast<StdKernel *>(_kernel.get())->_opt.kernel_radius = _data->_opt.kernel_radius;
	dynamic_cast<StdKernel *>(_kernel.get())->_rebuild_();
}
void HinaPE::SPHSolver::step(real dt)
{
	_opt.current_dt = dt;
	_update_density();
}
void HinaPE::SPHSolver::_update_density() const
{
	auto &p = _data->_positions;
	auto &d = _data->_densities;
	const auto &m = _data->_opt.mass;

	Util::parallelFor(static_cast<size_t>(0), _data->size(), [&](size_t i)
	{
		mVector3 origin = p[i];
		real sum = 0;
		_data->_neighbor_searcher->for_each_nearby_point(origin, _data->_opt.kernel_radius, [&](size_t, const mVector3 &neighbor_position)
		{
			real dist = (origin - neighbor_position).length();
			sum += (*_kernel)(dist); // Note: Don't use parallel for here
		});
		d[i] = m * sum;
	});
}
void HinaPE::SPHSolver::Data::_rebuild_()
{
	_neighbor_searcher = std::make_shared<PointSimpleListSearcher3>();
	_neighbor_searcher->build(_positions);

	_neighbor_lists.clear();
	_neighbor_lists.resize(_positions.size());
	for (int i = 0; i < _positions.size(); ++i)
	{
		mVector3 origin = _positions[i];
		_neighbor_lists[i].clear();

		_neighbor_searcher->for_each_nearby_point(origin, _opt.max_search_radius, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				_neighbor_lists[i].push_back(j);
		});
	}
}
auto HinaPE::SPHSolver::Data::size() const -> size_t
{
	if (_positions.size() != _velocities.size() && _positions.size() != _forces.size() && _positions.size() != _densities.size() && _positions.size() != _pressures.size())
		throw std::runtime_error("SPHSolver::Data::size() error");
	return _positions.size();
}
void HinaPE::StdKernel::_rebuild_()
{
	h = _opt.kernel_radius;
	h2 = h * h;
	h3 = h2 * h;
	h5 = h3 * h2;
}
auto HinaPE::StdKernel::operator()(real distance) const -> real
{
	if (distance * distance >= h2)
		return 0;

	real x = 1.0 - distance * distance / h2;
	return 315.0 / (64.0 * HinaPE::Constant::PI * h3) * x * x * x;
}
auto HinaPE::StdKernel::first_derivative(real distance) const -> real
{
	if (distance > h)
		return 0;

	real x = 1.0 - distance * distance / h2;
	return -945.0 / (32.0 * HinaPE::Constant::PI * h5) * distance * x * x;
}
auto HinaPE::StdKernel::second_derivative(real distance) const -> real
{
	if (distance * distance > h2)
		return 0;

	double x = distance * distance / h2;
	return 945.0 / (32.0 * HinaPE::Constant::PI * h5) * (1 - x) * (5 * x - 1);
}
auto HinaPE::StdKernel::gradient(const mVector3 &point) const -> mVector3
{
	real dist = point.length();
	if (dist > 0)
		return gradient(dist, point / dist);
	return mVector3::Zero();
}
auto HinaPE::StdKernel::gradient(real distance, const mVector3 &direction) const -> mVector3
{
	return -first_derivative(distance) * direction;
}
