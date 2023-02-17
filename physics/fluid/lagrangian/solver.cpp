#include "solver.h"

HinaPE::SPHSolver::SPHSolver() : _data(std::make_shared<Data>()), _current_dt(0.02)
{
	_data->_rebuild_();
}
void HinaPE::SPHSolver::step(real dt)
{
	_current_dt = dt;
	_update_density();
}
void HinaPE::SPHSolver::_update_density()
{
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
