#include "api.h"
void ParticleSystem::prepare()
{
	Api::prepare();
}
void ParticleSystem::step(float dt)
{
	Api::step(dt);
}
void ParticleSystem::ui_sidebar()
{
	Api::ui_sidebar();
}
void ParticleSystem::key(int key, int scancode, int action, int mods)
{
	Api::key(key, scancode, action, mods);
}
