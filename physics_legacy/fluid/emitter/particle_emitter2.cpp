#include "particle_emitter2.h"

#include "pch.h"
#include "parallel.h"

namespace HinaPE::Fluid
{

ParticleEmitter2::ParticleEmitter2() = default;

ParticleEmitter2::~ParticleEmitter2() = default;

auto ParticleEmitter2::target() const -> const ParticleSystemData2Ptr &
{
    return _particles;
}

void ParticleEmitter2::setTarget(const ParticleSystemData2Ptr &particles)
{
    _particles = particles;

    onSetTarget(particles);
}

auto ParticleEmitter2::isEnabled() const -> bool { return _isEnabled; }

void ParticleEmitter2::setIsEnabled(bool enabled) { _isEnabled = enabled; }

void ParticleEmitter2::update(double currentTimeInSeconds, double timeIntervalInSeconds)
{
    if (_onBeginUpdateCallback)
    {
        _onBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
    }

    onUpdate(currentTimeInSeconds, timeIntervalInSeconds);
}

void ParticleEmitter2::onSetTarget(const ParticleSystemData2Ptr &particles)
{
    UNUSED_VARIABLE(particles);
}

void ParticleEmitter2::setOnBeginUpdateCallback(const OnBeginUpdateCallback &callback)
{
    _onBeginUpdateCallback = callback;
}

}  // namespace HinaPE::Fluid
