#ifndef HINAPE_ANIMATION_H
#define HINAPE_ANIMATION_H

#include <chrono>
#include <memory>

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

class Animation
{
public:
	virtual void update(const Frame &frame) final { on_update(frame); }

protected:
	virtual void on_update(const Frame &frame) = 0;
};
using AnimationPtr = std::shared_ptr<Animation>;

class Timer final
{
public:
	inline auto duration() const -> float { return static_cast<float >(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _starting_point).count()) / 1000000.f; }
	inline void reset() { _starting_point = std::chrono::steady_clock::now(); }

public:
	Timer() { reset(); }

private:
	std::chrono::steady_clock::time_point _starting_point;
};
}

#endif //HINAPE_ANIMATION_H
