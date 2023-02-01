#ifndef HINAPE_ANIMATION_H
#define HINAPE_ANIMATION_H

#include <chrono>
#include <memory>
#include <cassert>

#ifdef HinaDebug
#include <iostream>
#endif

namespace Hina
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
#include "base/timer.h"
#endif

class Animation
{
public:
	virtual void update(const Frame &frame) final
	{
		assert(___VALID_CHECK___());

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

private:
	virtual auto ___VALID_CHECK___() -> bool { return true; }
};
using AnimationPtr = std::shared_ptr<Animation>;
}

#endif //HINAPE_ANIMATION_H
