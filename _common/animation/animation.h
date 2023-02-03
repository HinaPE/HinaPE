#ifndef HINAPE_ANIMATION_H
#define HINAPE_ANIMATION_H

#include <chrono>
#include <memory>
#include <cassert>

#ifdef HINAPE_DEBUG
#include "util/timer.h"
#include <iostream>
#endif

namespace HinaPE::Animation
{
struct Frame final
{
public:
	inline auto time() const -> float { return static_cast<float>(_index) * _time_step; }
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
	virtual void update(const Frame &frame) final
	{
#ifdef HINAPE_DEBUG
		VALID_CHECK(); // if not debug mode, this will be optimized out
#endif

#ifdef HINAPE_DEBUG
		timer.reset();
#endif

		on_update(frame);

#ifdef HINAPE_DEBUG
		timer.duration("Total Time");
#endif
	}
	virtual void VALID_CHECK() {}

protected:
	virtual void on_update(const Frame &frame) = 0;

private:
#ifdef HINAPE_DEBUG
	Util::Timer timer;
#endif
};
using AnimationPtr = std::shared_ptr<Animation>;
}

#endif //HINAPE_ANIMATION_H
