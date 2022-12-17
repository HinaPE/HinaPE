#ifndef HINAPE_ANIMATION_H
#define HINAPE_ANIMATION_H

#include <chrono>
#include <memory>

#ifdef HinaDebug
#include <iostream>
#endif

namespace HinaPE
{
struct Frame final
{
public:
	inline auto time() const -> float { return static_cast<float >(_index) * _time_step; }
	inline void advance() { ++_index; }
	inline void advance(int delta) { _index += delta; }

public:
	int _index = 0;
	float _time_step = 1.0f / 60.0f;
};
using FramePtr = std::shared_ptr<Frame>;

#ifdef HinaDebug
class Timer final
{
public:
	inline void duration(const std::string &info)
	{
		for (int i = 1; i < stack_depth; ++i)
			std::cout << "\t|";
		_last_duration = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _starting_point).count()) / 1000000.f;
		std::cout << info << ": " << _last_duration << "s" << std::endl;
	}
	inline void reset() { _starting_point = std::chrono::steady_clock::now(); }

public:
	static int stack_depth;
	Timer()
	{
		if (stack_depth == 0) { std::cout << "==================== Start Evaluation ====================" << std::endl; }
		reset();
		++stack_depth;
	}
	~Timer()
	{
		--stack_depth;
		if (stack_depth == 0) { std::cout << "==================== End Evaluation ====================" << std::endl; }
	}

public:
	std::chrono::steady_clock::time_point _starting_point;
	float _last_duration = 0;
};
using TimerPtr = std::shared_ptr<Timer>;
#endif

class Animation
{
public:
	virtual void update(const Frame &frame) final
	{
#ifdef HinaDebug
		timer.reset();
#endif

		on_update(frame);

#ifdef HinaDebug
		timer.duration("Total Time");
#endif
	}

protected:
	virtual void on_update(const Frame &frame) = 0;

public:
#ifdef HinaDebug
	Timer timer;
#endif
};
using AnimationPtr = std::shared_ptr<Animation>;
}

#endif //HINAPE_ANIMATION_H
