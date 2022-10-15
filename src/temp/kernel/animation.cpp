// Copyright (c) 2018 Doyub Kim
//
// I am making my contributions/submissions to this project solely in my
// personal capacity and am not conveying any rights to any intellectual
// property of any third parties.

#include "math_lib/pch.h"
#include "animation.h"
#include "timer.h"

#include "math_lib/private_helpers.h"

using namespace jet;

Frame::Frame() = default;

Frame::Frame(int newIndex, double newTimeIntervalInSeconds) : index(newIndex), timeIntervalInSeconds(newTimeIntervalInSeconds)
{
}

auto Frame::timeInSeconds() const -> double
{
    return index * timeIntervalInSeconds;
}

void Frame::advance()
{
    ++index;
}

void Frame::advance(int delta)
{
    index += delta;
}

auto Frame::operator++() -> Frame &
{
    advance();
    return *this;
}

auto Frame::operator++(int i) -> Frame
{
    UNUSED_VARIABLE(i);

    Frame result = *this;
    advance();
    return result;
}

Animation::Animation() = default;

Animation::~Animation() = default;

void Animation::update(const Frame &frame)
{
    Timer timer;

    JET_INFO << "Begin updating frame: " << frame.index << " timeIntervalInSeconds: " << frame.timeIntervalInSeconds << " (1/" << 1.0 / frame.timeIntervalInSeconds << ") seconds";

    onUpdate(frame);

    JET_INFO << "End updating frame (took " << timer.durationInSeconds() << " seconds)";
}
