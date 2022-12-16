#include "physics_animation.h"

#ifdef HinaDebug
int HinaPE::Timer::stack_depth = 0;
#endif

void HinaPE::PhysicsAnimation::on_update(const HinaPE::Frame &frame)
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

void HinaPE::PhysicsAnimation::update_physics(float time_step)
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

			on_update_physics(actual_time);

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

			on_update_physics(actual_time);

#ifdef HinaDebug
			timer.duration("On Update Physics");
#endif

			remaining_time -= actual_time;
			_opt._current_time += actual_time;
		}
	}
}
