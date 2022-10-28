#ifndef HINAPE_FLUID_ENGINE_VOLUME_PARTICLE_EMITTER3_H_
#define HINAPE_FLUID_ENGINE_VOLUME_PARTICLE_EMITTER3_H_

#include "particle_emitter3.h"

#include "geometry/bounding_box3.h"
#include "geometry/implicit_surface3.h"
#include "point_generator/point_generator3.h"

#include <limits>
#include <memory>
#include <random>

namespace HinaPE::FluidEngine
{

//!
//! \brief 3-D volumetric particle emitter.
//!
//! This class emits particles from volumetric geometry.
//!
class VolumeParticleEmitter3 final : public ParticleEmitter3
{
public:
    class Builder;

    //!
    //! Constructs an emitter that spawns particles from given implicit surface
    //! which defines the volumetric geometry. Provided bounding box limits
    //! the particle generation region.
    //!
    //! \param[in]  implicitSurface         The implicit surface.
    //! \param[in]  maxRegion               The max region.
    //! \param[in]  spacing                 The spacing between particles.
    //! \param[in]  initialVel              The initial velocity.
    //! \param[in]  linearVel               The linear velocity of the emitter.
    //! \param[in]  angularVel              The angular velocity of the emitter.
    //! \param[in]  maxNumberOfParticles    The max number of particles to be
    //!                                     emitted.
    //! \param[in]  jitter                  The jitter amount between 0 and 1.
    //! \param[in]  isOneShot               True if emitter gets disabled after one shot.
    //! \param[in]  allowOverlapping        True if particles can be overlapped.
    //! \param[in]  seed                    The random seed.
    //!
    VolumeParticleEmitter3(const ImplicitSurface3Ptr &implicitSurface, const BoundingBox3D &maxRegion, double spacing, const Vector3D &initialVel = Vector3D(), const Vector3D &linearVel = Vector3D(), const Vector3D &angularVel = Vector3D(),
                           size_t maxNumberOfParticles = kMaxSize, double jitter = 0.0, bool isOneShot = true, bool allowOverlapping = false, uint32_t seed = 0);

    //!
    //! \brief      Sets the point generator.
    //!
    //! This function sets the point generator that defines the pattern of the
    //! point distribution within the volume.
    //!
    //! \param[in]  newPointsGen The new points generator.
    //!
    void setPointGenerator(const PointGenerator3Ptr &newPointsGen);

    //! Returns source surface.
    auto surface() const -> const ImplicitSurface3Ptr &;

    //! Sets the source surface.
    void setSurface(const ImplicitSurface3Ptr &newSurface);

    //! Returns max particle gen region.
    auto maxRegion() const -> const BoundingBox3D &;

    //! Sets the max particle gen region.
    void setMaxRegion(const BoundingBox3D &newBox);

    //! Returns jitter amount.
    auto jitter() const -> double;

    //! Sets jitter amount between 0 and 1.
    void setJitter(double newJitter);

    //! Returns true if particles should be emitted just once.
    auto isOneShot() const -> bool;

    //!
    //! \brief      Sets the flag to true if particles are emitted just once.
    //!
    //! If true is set, the emitter will generate particles only once even after
    //! multiple emit calls. If false, it will keep generating particles from
    //! the volumetric geometry. Default value is true.
    //!
    //! \param[in]  newValue True if particles should be emitted just once.
    //!
    void setIsOneShot(bool newValue);

    //! Returns true if particles can be overlapped.
    auto allowOverlapping() const -> bool;

    //!
    //! \brief      Sets the flag to true if particles can overlap each other.
    //!
    //! If true is set, the emitter will generate particles even if the new
    //! particles can find existing nearby particles within the particle
    //! spacing.
    //!
    //! \param[in]  newValue True if particles can be overlapped.
    //!
    void setAllowOverlapping(bool newValue);

    //! Returns max number of particles to be emitted.
    auto maxNumberOfParticles() const -> size_t;

    //! Sets the max number of particles to be emitted.
    void setMaxNumberOfParticles(size_t newMaxNumberOfParticles);

    //! Returns the spacing between particles.
    auto spacing() const -> double;

    //! Sets the spacing between particles.
    void setSpacing(double newSpacing);

    //! Sets the initial velocity of the particles.
    auto initialVelocity() const -> Vector3D;

    //! Returns the initial velocity of the particles.
    void setInitialVelocity(const Vector3D &newInitialVel);

    //! Returns the linear velocity of the emitter.
    auto linearVelocity() const -> Vector3D;

    //! Sets the linear velocity of the emitter.
    void setLinearVelocity(const Vector3D &newLinearVel);

    //! Returns the angular velocity of the emitter.
    auto angularVelocity() const -> Vector3D;

    //! Sets the linear velocity of the emitter.
    void setAngularVelocity(const Vector3D &newAngularVel);

    //! Returns builder fox VolumeParticleEmitter3.
    static auto builder() -> Builder;

private:
    std::mt19937 _rng;

    ImplicitSurface3Ptr _implicitSurface;
    BoundingBox3D _bounds;
    double _spacing;
    Vector3D _initialVel;
    Vector3D _linearVel;
    Vector3D _angularVel;
    PointGenerator3Ptr _pointsGen;

    size_t _maxNumberOfParticles = kMaxSize;
    size_t _numberOfEmittedParticles = 0;

    double _jitter = 0.0;
    bool _isOneShot = true;
    bool _allowOverlapping = false;

    //!
    //! \brief      Emits particles to the particle system data.
    //!
    //! \param[in]  currentTimeInSeconds    Current simulation time.
    //! \param[in]  timeIntervalInSeconds   The time-step interval.
    //!
    void onUpdate(double currentTimeInSeconds, double timeIntervalInSeconds) override;

    void emit(const ParticleSystemData3Ptr &particles, Array1<Vector3D> *newPositions, Array1<Vector3D> *newVelocities);

    auto random() -> double;

    auto velocityAt(const Vector3D &point) const -> Vector3D;
};

//! Shared pointer for the VolumeParticleEmitter3 type.
using VolumeParticleEmitter3Ptr = std::shared_ptr<VolumeParticleEmitter3>;

//!
//! \brief Front-end to create VolumeParticleEmitter3 objects step by step.
//!
class VolumeParticleEmitter3::Builder final
{
public:
    //! Returns builder with implicit surface defining volume shape.
    auto withImplicitSurface(const ImplicitSurface3Ptr &implicitSurface) -> Builder &;

    //! Returns builder with surface defining volume shape.
    auto withSurface(const Surface3Ptr &surface) -> Builder &;

    //! Returns builder with max region.
    auto withMaxRegion(const BoundingBox3D &bounds) -> Builder &;

    //! Returns builder with spacing.
    auto withSpacing(double spacing) -> Builder &;

    //! Returns builder with initial velocity.
    auto withInitialVelocity(const Vector3D &initialVel) -> Builder &;

    //! Returns builder with linear velocity.
    auto withLinearVelocity(const Vector3D &linearVel) -> Builder &;

    //! Returns builder with angular velocity.
    auto withAngularVelocity(const Vector3D &angularVel) -> Builder &;

    //! Returns builder with max number of particles.
    auto withMaxNumberOfParticles(size_t maxNumberOfParticles) -> Builder &;

    //! Returns builder with jitter amount.
    auto withJitter(double jitter) -> Builder &;

    //! Returns builder with one-shot flag.
    auto withIsOneShot(bool isOneShot) -> Builder &;

    //! Returns builder with overlapping flag.
    auto withAllowOverlapping(bool allowOverlapping) -> Builder &;

    //! Returns builder with random seed.
    auto withRandomSeed(uint32_t seed) -> Builder &;

    //! Builds VolumeParticleEmitter3.
    auto build() const -> VolumeParticleEmitter3;

    //! Builds shared pointer of VolumeParticleEmitter3 instance.
    auto makeShared() const -> VolumeParticleEmitter3Ptr;

private:
    ImplicitSurface3Ptr _implicitSurface;
    bool _isBoundSet = false;
    BoundingBox3D _bounds;
    double _spacing = 0.1;
    Vector3D _initialVel;
    Vector3D _linearVel;
    Vector3D _angularVel;
    size_t _maxNumberOfParticles = kMaxSize;
    double _jitter = 0.0;
    bool _isOneShot = true;
    bool _allowOverlapping = false;
    uint32_t _seed = 0;
};

}  // namespace HinaPE::FluidEngine

#endif  // HINAPE_FLUID_ENGINE_VOLUME_PARTICLE_EMITTER3_H_
