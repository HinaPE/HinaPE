#ifndef INCLUDE_JET_SPH_SYSTEM_DATA3_H_
#define INCLUDE_JET_SPH_SYSTEM_DATA3_H_

#include "../particle_system_data3.h"

#include "constants.h"

#include <vector>
#include <limits>

namespace jet
{

//!
//! \brief      3-D SPH particle system data.
//!
//! This class extends ParticleSystemData3 to specialize the data model for SPH.
//! It includes density and pressure array as a default particle attribute, and
//! it also contains SPH utilities such as interpolation operator.
//!
class SphSystemData3 : public ParticleSystemData3
{
public:
    //! Constructs empty SPH system.
    SphSystemData3();

    //! Constructs SPH system data with given number of particles.
    explicit SphSystemData3(size_t numberOfParticles);

    //! Copy constructor.
    SphSystemData3(const SphSystemData3 &other);

    //! Destructor.
    ~SphSystemData3() override;

    //!
    //! \brief Sets the radius.
    //!
    //! Sets the radius of the particle system. The radius will be interpreted
    //! as target spacing.
    //!
    void setRadius(double newRadius) override;

    //!
    //! \brief      Sets the mass of a particle.
    //!
    //! Setting the mass of a particle will change the target density.
    //!
    //! \param[in]  newMass The new mass.
    //!
    void setMass(double newMass) override;

    //! Returns the density array accessor (immutable).
    auto densities() const -> ConstArrayAccessor1<double>;

    //! Returns the density array accessor (mutable).
    auto densities() -> ArrayAccessor1<double>;

    //! Returns the pressure array accessor (immutable).
    auto pressures() const -> ConstArrayAccessor1<double>;

    //! Returns the pressure array accessor (mutable).
    auto pressures() -> ArrayAccessor1<double>;

    //! Updates the density array with the latest particle positions.
    void updateDensities();

    //! Sets the target density of this particle system.
    void setTargetDensity(double targetDensity);

    //! Returns the target density of this particle system.
    auto targetDensity() const -> double;

    //!
    //! \brief Sets the target particle spacing in meters.
    //!
    //! Once this function is called, hash grid and density should be
    //! updated using updateHashGrid() and updateDensities).
    //!
    void setTargetSpacing(double spacing);

    //! Returns the target particle spacing in meters.
    auto targetSpacing() const -> double;

    //!
    //! \brief Sets the relative kernel radius.
    //!
    //! Sets the relative kernel radius compared to the target particle
    //! spacing (i.e. kernel radius / target spacing).
    //! Once this function is called, hash grid and density should
    //! be updated using updateHashGrid() and updateDensities).
    //!
    void setRelativeKernelRadius(double relativeRadius);

    //!
    //! \brief Sets the absolute kernel radius.
    //!
    //! Sets the absolute kernel radius compared to the target particle
    //! spacing (i.e. relative kernel radius * target spacing).
    //! Once this function is called, hash grid and density should
    //! be updated using updateHashGrid() and updateDensities).
    //!
    void setKernelRadius(double kernelRadius);

    //!
    //! \brief Returns the relative kernel radius.
    //!
    //! Returns the relative kernel radius compared to the target particle
    //! spacing (i.e. kernel radius / target spacing).
    //!
    auto relativeKernelRadius() const -> double;

    //! Returns the kernel radius in meters unit.
    auto kernelRadius() const -> double;

    //! Returns sum of kernel function evaluation for each nearby particle.
    auto sumOfKernelNearby(const Vector3D &position) const -> double;

    //!
    //! \brief Returns interpolated value at given origin point.
    //!
    //! Returns interpolated scalar data from the given position using
    //! standard SPH weighted average. The data array should match the
    //! particle layout. For example, density or pressure arrays can be
    //! used.
    //!
    auto interpolate(const Vector3D &origin, const ConstArrayAccessor1<double> &values) const -> double;

    //!
    //! \brief Returns interpolated vector value at given origin point.
    //!
    //! Returns interpolated vector data from the given position using
    //! standard SPH weighted average. The data array should match the
    //! particle layout. For example, velocity or acceleration arrays can be
    //! used.
    //!
    auto interpolate(const Vector3D &origin, const ConstArrayAccessor1<Vector3D> &values) const -> Vector3D;

    //! Returns the gradient of the given values at i-th particle.
    auto gradientAt(size_t i, const ConstArrayAccessor1<double> &values) const -> Vector3D;

    //! Returns the laplacian of the given values at i-th particle.
    auto laplacianAt(size_t i, const ConstArrayAccessor1<double> &values) const -> double;

    //! Returns the laplacian of the given values at i-th particle.
    auto laplacianAt(size_t i, const ConstArrayAccessor1<Vector3D> &values) const -> Vector3D;

    //! Builds neighbor searcher with kernel radius.
    void buildNeighborSearcher();

    //! Builds neighbor lists with kernel radius.
    void buildNeighborLists();

    //! Serializes this SPH system data to the buffer.
    void serialize(std::vector<uint8_t> *buffer) const override;

    //! Deserializes this SPH system data from the buffer.
    void deserialize(const std::vector<uint8_t> &buffer) override;

    //! Copies from other SPH system data.
    void set(const SphSystemData3 &other);

    //! Copies from other SPH system data.
    auto operator=(const SphSystemData3 &other) -> SphSystemData3 &;

private:
    //! Target density of this particle system in kg/m^3.
    double _targetDensity = kWaterDensity;

    //! Target spacing of this particle system in meters.
    double _targetSpacing = 0.1;

    //! Relative radius of SPH kernel.
    //! SPH kernel radius divided by target spacing.
    double _kernelRadiusOverTargetSpacing = 1.8;

    //! SPH kernel radius in meters.
    double _kernelRadius = std::numeric_limits<double>::min(); // to make cpplint happy

    size_t _pressureIdx = std::numeric_limits<size_t>::max(); // to make cpplint happy

    size_t _densityIdx = std::numeric_limits<size_t>::max(); // to make cpplint happy

    //! Computes the mass based on the target density and spacing.
    void computeMass();
};

//! Shared pointer for the SphSystemData3 type.
using SphSystemData3Ptr = std::shared_ptr<SphSystemData3>;

}  // namespace jet

#endif  // INCLUDE_JET_SPH_SYSTEM_DATA3_H_
