#ifndef HINAPE_PHYSICS_ANIMATION_H
#define HINAPE_PHYSICS_ANIMATION_H

#include "animation.h"

namespace HinaPE
{
class PhysicsAnimation : public Animation
{
public:
	inline void advance_a_frame() { update(Frame(_current_frame)); }

public:
	struct Opt
	{
		bool fix_sub_time_step = false;
		int sub_time_step = 1;
	} _opt;

protected:
	virtual void on_init_physics() = 0;
	virtual void on_update_physics(float time_step) = 0;

private:
	void on_update(const Frame &frame) final;
	virtual inline void init_physics() final { on_init_physics(); };
	virtual void update_physics(float time_step) final;

private:
	Frame _current_frame;
	float _current_time = 0.f;
};
using PhysicsAnimationPtr = std::shared_ptr<PhysicsAnimation>;
}

#endif //HINAPE_PHYSICS_ANIMATION_H
