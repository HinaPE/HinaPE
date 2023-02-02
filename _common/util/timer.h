#ifndef HINAPE_TIMER_H
#define HINAPE_TIMER_H

#include <iostream>
#include <chrono>
#include <memory>

namespace HinaPE::Util
{
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
}
#endif //HINAPE_TIMER_H
