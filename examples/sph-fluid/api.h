#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/api.h"
#include "renderer/scene/scene.h"
#include "fluid/solver/particle/sph/sph_solver3.h"

namespace HinaPE::Fluid
{
class Api : public Kasumi::Api
{
public:
	void step(float dt) final;
	void ui_sidebar() final;
	void sync();

private:

};
}

#endif //HINAPE_API_H
