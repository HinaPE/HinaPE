#ifndef HINAPE_PHYSICS_ANIMATION_H
#define HINAPE_PHYSICS_ANIMATION_H

#include "animation.h"

namespace Hina
{
class PhysicsAnimation : public Animation
{
public:
	inline void on_update(const Frame &frame) final;
	inline void advance_a_frame()
	{
		Frame f(_current_frame);
		f.advance();
		update(f);
	}

public:
	struct Opt
	{
		float _current_time = 0.f;

		bool fix_sub_time_step = false;
		int sub_time_step = 1;
	} _opt;

protected:
	virtual void on_physics_init() = 0;
	virtual void on_physics_update(float time_step) = 0;

private:
	virtual inline void init_physics() final { on_physics_init(); };
	virtual inline void update_physics(float time_step) final;

private:
	Frame _current_frame;
};
using PhysicsAnimationPtr = std::shared_ptr<PhysicsAnimation>;

void Hina::PhysicsAnimation::on_update(const Hina::Frame &frame)
{
	if (frame._index > _current_frame._index)
	{
		if (_current_frame._index <= 0)
			init_physics();

		int frame_num = frame._index - _current_frame._index;

		for (int i = 0; i < frame_num; ++i)
			update_physics(frame._time_step);

		_current_frame = frame;
	}
}

void Hina::PhysicsAnimation::update_physics(float time_step)
{
	_opt._current_time = _current_frame.time();

	if (_opt.fix_sub_time_step)
	{
		const float actual_time = time_step / static_cast<float>(_opt.sub_time_step);

		for (int i = 0; i < _opt.sub_time_step; ++i)
		{
#ifdef HinaDebug
			Timer timer;
#endif

			on_physics_update(actual_time);

#ifdef HinaDebug
			timer.duration("On Update Physics");
#endif

			_opt._current_time += actual_time;
		}
	} else
	{
		float remaining_time = time_step;
		while (remaining_time > std::numeric_limits<float>::epsilon())
		{
			float actual_time = remaining_time / static_cast<float>(_opt.sub_time_step);

#ifdef HinaDebug
			Timer timer;
#endif

			on_physics_update(actual_time);

#ifdef HinaDebug
			timer.duration("On Update Physics");
#endif

			remaining_time -= actual_time;
			_opt._current_time += actual_time;
		}
	}
}
}

#endif //HINAPE_PHYSICS_ANIMATION_H
