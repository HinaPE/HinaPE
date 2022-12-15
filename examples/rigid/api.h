#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/app.h"
#include "solver/rigid_solver.h"

class Api : public Kasumi::Api
{
public:
	void prepare() final;
	void step(float dt) final;
	void ui_sidebar() final;
	void sync();

private:
	std::vector<std::pair<Kasumi::SceneObjectPtr, unsigned int>> _objs; // scene object, rigid body id
};


#endif //HINAPE_API_H
