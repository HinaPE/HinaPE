#include "cloud_solver.h"

CloudSolver::Data::Data(const mVector3 &size, const mSize3 &resolution, const mVector3 &center)
{
	auto spacing = {
			size.x() / static_cast<real>(resolution.x),
			size.y() / static_cast<real>(resolution.y),
			size.z() / static_cast<real>(resolution.z)};
	Cloud.hum.resize(resolution, spacing, center);
	Cloud.cld.resize(resolution, spacing, center);
	Cloud.act.resize(resolution, spacing, center);

	track(&Cloud.cld);
}
void CloudSolver::Data::reset()
{
}
void CloudSolver::init()
{
	if (_data == nullptr)
		_data = std::make_shared<Data>(mVector3(2, 2, 2), mSize3(50, 50, 50));
	if (_domain == nullptr)
		_domain = std::make_shared<BoxDomain>();
}
void CloudSolver::update(real dt) const
{
}
void CloudSolver::reset()
{
}
