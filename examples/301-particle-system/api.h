#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/app.h"

class ParticleSystem : public Kasumi::Api
{
public:
	void prepare() final;
	void step(float dt) final;
	void ui_sidebar() final;
	void key(int key, int scancode, int action, int mods) final;
};


#endif //HINAPE_API_H
