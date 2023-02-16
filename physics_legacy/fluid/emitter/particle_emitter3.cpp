#include "particle_emitter3.h"

#include "pch.h"
#include "parallel.h"

#include <limits>

namespace HinaPE::Fluid
{

ParticleEmitter3::ParticleEmitter3() = default;

ParticleEmitter3::~ParticleEmitter3() = default;

auto ParticleEmitter3::target() const -> const ParticleSystemData3Ptr &
{
    return _particles;
}

void ParticleEmitter3::setTarget(const ParticleSystemData3Ptr &particles)
{
    _particles = particles;

    onSetTarget(particles);
}

auto ParticleEmitter3::isEnabled() const -> bool { return _isEnabled; }

void ParticleEmitter3::setIsEnabled(bool enabled) { _isEnabled = enabled; }

void ParticleEmitter3::update(double currentTimeInSeconds, double timeIntervalInSeconds)
{
    if (_onBeginUpdateCallback)
    {
        _onBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
    }

    onUpdate(currentTimeInSeconds, timeIntervalInSeconds);
}

void ParticleEmitter3::onSetTarget(const ParticleSystemData3Ptr &particles)
{
    UNUSED_VARIABLE(particles);
}

void ParticleEmitter3::setOnBeginUpdateCallback(const OnBeginUpdateCallback &callback)
{
    _onBeginUpdateCallback = callback;
}

}  // namespace HinaPE::Fluid
