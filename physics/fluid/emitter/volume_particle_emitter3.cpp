#include "volume_particle_emitter3.h"

#include "point_generator/bcc_lattice_point_generator.h"
#include "searcher/point_hash_grid_searcher3.h"
#include "geometry/surface_to_implicit3.h"

#include "pch.h"
#include "samplers.h"

using namespace HinaPE;
using namespace HinaPE::Fluid;

static const size_t kDefaultHashGridResolution = 64;

VolumeParticleEmitter3::VolumeParticleEmitter3(const ImplicitSurface3Ptr &implicitSurface, const BoundingBox3D &maxRegion, double spacing, const Vector3D &initialVel, const Vector3D &linearVel, const Vector3D &angularVel, size_t maxNumberOfParticles, double jitter,
                                               bool isOneShot, bool allowOverlapping, uint32_t seed) : _rng(seed), _implicitSurface(implicitSurface), _bounds(maxRegion), _spacing(spacing), _initialVel(initialVel), _linearVel(linearVel), _angularVel(angularVel),
                                                                                                       _maxNumberOfParticles(maxNumberOfParticles), _jitter(jitter), _isOneShot(isOneShot), _allowOverlapping(allowOverlapping)
{
    _pointsGen = std::make_shared<BccLatticePointGenerator>();
}

void VolumeParticleEmitter3::onUpdate(double currentTimeInSeconds, double timeIntervalInSeconds)
{
    UNUSED_VARIABLE(currentTimeInSeconds);
    UNUSED_VARIABLE(timeIntervalInSeconds);

    auto particles = target();

    if (particles == nullptr)
    {
        return;
    }

    if (!isEnabled())
    {
        return;
    }

    Array1<Vector3D> newPositions;
    Array1<Vector3D> newVelocities;

    emit(particles, &newPositions, &newVelocities);

    particles->addParticles(newPositions, newVelocities);

    if (_isOneShot)
    {
        setIsEnabled(false);
    }
}

void VolumeParticleEmitter3::emit(const ParticleSystemData3Ptr &particles, Array1<Vector3D> *newPositions, Array1<Vector3D> *newVelocities)
{
    if (!_implicitSurface)
    {
        return;
    }

    _implicitSurface->updateQueryEngine();

    BoundingBox3D region = _bounds;
    if (_implicitSurface->isBounded())
    {
        BoundingBox3D surfaceBBox = _implicitSurface->boundingBox();
        region.lowerCorner = max(region.lowerCorner, surfaceBBox.lowerCorner);
        region.upperCorner = min(region.upperCorner, surfaceBBox.upperCorner);
    }

    // Reserving more space for jittering
    const double j = jitter();
    const double maxJitterDist = 0.5 * j * _spacing;
    size_t numNewParticles = 0;

    if (_allowOverlapping || _isOneShot)
    {
        _pointsGen->forEachPoint(region, _spacing, [&](const Vector3D &point)
        {
            Vector3D randomDir = uniformSampleSphere(random(), random());
            Vector3D offset = maxJitterDist * randomDir;
            Vector3D candidate = point + offset;
            if (_implicitSurface->signedDistance(candidate) <= 0.0)
            {
                if (_numberOfEmittedParticles < _maxNumberOfParticles)
                {
                    newPositions->append(candidate);
                    ++_numberOfEmittedParticles;
                    ++numNewParticles;
                } else
                {
                    return false;
                }
            }

            return true;
        });
    } else
    {
        // Use serial hash grid searcher for continuous update.
        PointHashGridSearcher3 neighborSearcher(Size3(kDefaultHashGridResolution, kDefaultHashGridResolution, kDefaultHashGridResolution), 2.0 * _spacing);
        if (!_allowOverlapping)
        {
            neighborSearcher.build(particles->positions());
        }

        _pointsGen->forEachPoint(region, _spacing, [&](const Vector3D &point)
        {
            Vector3D randomDir = uniformSampleSphere(random(), random());
            Vector3D offset = maxJitterDist * randomDir;
            Vector3D candidate = point + offset;
            if (_implicitSurface->isInside(candidate) && (!_allowOverlapping && !neighborSearcher.hasNearbyPoint(candidate, _spacing)))
            {
                if (_numberOfEmittedParticles < _maxNumberOfParticles)
                {
                    newPositions->append(candidate);
                    neighborSearcher.add(candidate);
                    ++_numberOfEmittedParticles;
                    ++numNewParticles;
                } else
                {
                    return false;
                }
            }

            return true;
        });
    }

    JET_INFO << "Number of newly generated particles: " << numNewParticles;
    JET_INFO << "Number of total generated particles: " << _numberOfEmittedParticles;

    newVelocities->resize(newPositions->size());
    newVelocities->parallelForEachIndex([&](size_t i)
                                        {
                                            (*newVelocities)[i] = velocityAt((*newPositions)[i]);
                                        });
}

void VolumeParticleEmitter3::setPointGenerator(const PointGenerator3Ptr &newPointsGen)
{
    _pointsGen = newPointsGen;
}

auto VolumeParticleEmitter3::surface() const -> const ImplicitSurface3Ptr &
{
    return _implicitSurface;
}

void VolumeParticleEmitter3::setSurface(const ImplicitSurface3Ptr &newSurface)
{
    _implicitSurface = newSurface;
}

auto VolumeParticleEmitter3::maxRegion() const -> const BoundingBox3D &
{
    return _bounds;
}

void VolumeParticleEmitter3::setMaxRegion(const BoundingBox3D &newMaxRegion)
{
    _bounds = newMaxRegion;
}

auto VolumeParticleEmitter3::jitter() const -> double { return _jitter; }

void VolumeParticleEmitter3::setJitter(double newJitter)
{
    _jitter = clamp(newJitter, 0.0, 1.0);
}

auto VolumeParticleEmitter3::isOneShot() const -> bool { return _isOneShot; }

void VolumeParticleEmitter3::setIsOneShot(bool newValue)
{
    _isOneShot = newValue;
}

auto VolumeParticleEmitter3::allowOverlapping() const -> bool
{
    return _allowOverlapping;
}

void VolumeParticleEmitter3::setAllowOverlapping(bool newValue)
{
    _allowOverlapping = newValue;
}

auto VolumeParticleEmitter3::maxNumberOfParticles() const -> size_t
{
    return _maxNumberOfParticles;
}

void VolumeParticleEmitter3::setMaxNumberOfParticles(size_t newMaxNumberOfParticles)
{
    _maxNumberOfParticles = newMaxNumberOfParticles;
}

auto VolumeParticleEmitter3::spacing() const -> double { return _spacing; }

void VolumeParticleEmitter3::setSpacing(double newSpacing)
{
    _spacing = newSpacing;
}

auto VolumeParticleEmitter3::initialVelocity() const -> Vector3D { return _initialVel; }

void VolumeParticleEmitter3::setInitialVelocity(const Vector3D &newInitialVel)
{
    _initialVel = newInitialVel;
}

auto VolumeParticleEmitter3::linearVelocity() const -> Vector3D { return _linearVel; }

void VolumeParticleEmitter3::setLinearVelocity(const Vector3D &newLinearVel)
{
    _linearVel = newLinearVel;
}

auto VolumeParticleEmitter3::angularVelocity() const -> Vector3D { return _angularVel; }

void VolumeParticleEmitter3::setAngularVelocity(const Vector3D &newAngularVel)
{
    _angularVel = newAngularVel;
}

auto VolumeParticleEmitter3::random() -> double
{
    std::uniform_real_distribution<> d(0.0, 1.0);
    return d(_rng);
}

auto VolumeParticleEmitter3::velocityAt(const Vector3D &point) const -> Vector3D
{
    Vector3D r = point - _implicitSurface->transform.translation();
    return _linearVel + _angularVel.cross(r) + _initialVel;
}

auto VolumeParticleEmitter3::builder() -> VolumeParticleEmitter3::Builder
{
    return {};
}

auto VolumeParticleEmitter3::Builder::withImplicitSurface(const ImplicitSurface3Ptr &implicitSurface) -> VolumeParticleEmitter3::Builder &
{
    _implicitSurface = implicitSurface;
    if (!_isBoundSet)
    {
        _bounds = _implicitSurface->boundingBox();
    }
    return *this;
}

auto VolumeParticleEmitter3::Builder::withSurface(const Surface3Ptr &surface) -> VolumeParticleEmitter3::Builder &
{
    _implicitSurface = std::make_shared<SurfaceToImplicit3>(surface);
    if (!_isBoundSet)
    {
        _bounds = surface->boundingBox();
    }
    return *this;
}

auto VolumeParticleEmitter3::Builder::withMaxRegion(const BoundingBox3D &bounds) -> VolumeParticleEmitter3::Builder &
{
    _bounds = bounds;
    _isBoundSet = true;
    return *this;
}

auto VolumeParticleEmitter3::Builder::withSpacing(double spacing) -> VolumeParticleEmitter3::Builder &
{
    _spacing = spacing;
    return *this;
}

auto VolumeParticleEmitter3::Builder::withInitialVelocity(const Vector3D &initialVel) -> VolumeParticleEmitter3::Builder &
{
    _initialVel = initialVel;
    return *this;
}

auto VolumeParticleEmitter3::Builder::withLinearVelocity(const Vector3D &linearVel) -> VolumeParticleEmitter3::Builder &
{
    _linearVel = linearVel;
    return *this;
}

auto VolumeParticleEmitter3::Builder::withAngularVelocity(const Vector3D &angularVel) -> VolumeParticleEmitter3::Builder &
{
    _angularVel = angularVel;
    return *this;
}

auto VolumeParticleEmitter3::Builder::withMaxNumberOfParticles(size_t maxNumberOfParticles) -> VolumeParticleEmitter3::Builder &
{
    _maxNumberOfParticles = maxNumberOfParticles;
    return *this;
}

auto VolumeParticleEmitter3::Builder::withJitter(double jitter) -> VolumeParticleEmitter3::Builder &
{
    _jitter = jitter;
    return *this;
}

auto VolumeParticleEmitter3::Builder::withIsOneShot(bool isOneShot) -> VolumeParticleEmitter3::Builder &
{
    _isOneShot = isOneShot;
    return *this;
}

auto VolumeParticleEmitter3::Builder::withAllowOverlapping(bool allowOverlapping) -> VolumeParticleEmitter3::Builder &
{
    _allowOverlapping = allowOverlapping;
    return *this;
}

auto VolumeParticleEmitter3::Builder::withRandomSeed(uint32_t seed) -> VolumeParticleEmitter3::Builder &
{
    _seed = seed;
    return *this;
}

auto VolumeParticleEmitter3::Builder::build() const -> VolumeParticleEmitter3
{
    return VolumeParticleEmitter3(_implicitSurface, _bounds, _spacing, _initialVel, _linearVel, _angularVel, _maxNumberOfParticles, _jitter, _isOneShot, _allowOverlapping, _seed);
}

auto VolumeParticleEmitter3::Builder::makeShared() const -> VolumeParticleEmitter3Ptr
{
    return std::shared_ptr<VolumeParticleEmitter3>(new VolumeParticleEmitter3(_implicitSurface, _bounds, _spacing, _initialVel, _linearVel, _angularVel, _maxNumberOfParticles, _jitter, _isOneShot, _allowOverlapping), [](VolumeParticleEmitter3 *obj) { delete obj; });
}
