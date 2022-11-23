#ifndef HINAPE_FLUID_ENGINE_PARTICLE_SYSTEM_DATA3_H_
#define HINAPE_FLUID_ENGINE_PARTICLE_SYSTEM_DATA3_H_

#include "searcher/point_neighbor_searcher3.h"
#include "array/array1.h"

#include "serialization.h"

#include <memory>
#include <vector>

#ifndef JET_DOXYGEN

namespace flatbuffers
{
class FlatBufferBuilder;
template<typename T>
struct Offset;
}

namespace HinaPE::fbs
{
struct ParticleSystemData3;
}

#endif  // JET_DOXYGEN

namespace HinaPE::FluidEngine
{

//!
//! \brief      3-D particle system data.
//!
//! This class is the key data structure for storing particle system data. A
//! single particle has position, velocity, and force attributes by default. But
//! it can also have additional custom scalar or vector attributes.
//!
class ParticleSystemData3 : public Serializable
{
public:
    //! Scalar data chunk.
    using ScalarData = Array1<double>;

    //! Vector data chunk.
    using VectorData = Array1<Vector3D>;

    //! Default constructor.
    ParticleSystemData3();

    //! Constructs particle system data with given number of particles.
    explicit ParticleSystemData3(size_t numberOfParticles);

    //! Copy constructor.
    ParticleSystemData3(const ParticleSystemData3 &other);

    //! Destructor.
    ~ParticleSystemData3() override;

    //!
    //! \brief      Resizes the number of particles of the container.
    //!
    //! This function will resize internal containers to store newly given
    //! number of particles including custom data layers. However, this will
    //! invalidate neighbor searcher and neighbor lists. It is users
    //! responsibility to call ParticleSystemData3::buildNeighborSearcher and
    //! ParticleSystemData3::buildNeighborLists to refresh those data.
    //!
    //! \param[in]  newNumberOfParticles    New number of particles.
    //!
    void resize(size_t newNumberOfParticles);

    //! Returns the number of particles.
    auto numberOfParticles() const -> size_t;

    //!
    //! \brief      Adds a scalar data layer and returns its index.
    //!
    //! This function adds a new scalar data layer to the system. It can be used
    //! for adding a scalar attribute, such as temperature, to the particles.
    //!
    //! \params[in] initialVal  Initial value of the new scalar data.
    //!
    auto addScalarData(double initialVal = 0.0) -> size_t;

    //!
    //! \brief      Adds a vector data layer and returns its index.
    //!
    //! This function adds a new vector data layer to the system. It can be used
    //! for adding a vector attribute, such as vortex, to the particles.
    //!
    //! \params[in] initialVal  Initial value of the new vector data.
    //!
    auto addVectorData(const Vector3D &initialVal = Vector3D()) -> size_t;

    //! Returns the radius of the particles.
    auto radius() const -> double;

    //! Sets the radius of the particles.
    virtual void setRadius(double newRadius);

    //! Returns the mass of the particles.
    auto mass() const -> double;

    //! Sets the mass of the particles.
    virtual void setMass(double newMass);

    //! Returns the position array (immutable).
    auto positions() const -> ConstArrayAccessor1<Vector3D>;

    //! Returns the position array (mutable).
    auto positions() -> ArrayAccessor1<Vector3D>;

    //! Returns the velocity array (immutable).
    auto velocities() const -> ConstArrayAccessor1<Vector3D>;

    //! Returns the velocity array (mutable).
    auto velocities() -> ArrayAccessor1<Vector3D>;

    //! Returns the force array (immutable).
    auto forces() const -> ConstArrayAccessor1<Vector3D>;

    //! Returns the force array (mutable).
    auto forces() -> ArrayAccessor1<Vector3D>;

    //! Returns custom scalar data layer at given index (immutable).
    auto scalarDataAt(size_t idx) const -> ConstArrayAccessor1<double>;

    //! Returns custom scalar data layer at given index (mutable).
    auto scalarDataAt(size_t idx) -> ArrayAccessor1<double>;

    //! Returns custom vector data layer at given index (immutable).
    auto vectorDataAt(size_t idx) const -> ConstArrayAccessor1<Vector3D>;

    //! Returns custom vector data layer at given index (mutable).
    auto vectorDataAt(size_t idx) -> ArrayAccessor1<Vector3D>;

    //!
    //! \brief      Adds a particle to the data structure.
    //!
    //! This function will add a single particle to the data structure. For
    //! custom data layers, zeros will be assigned for new particles.
    //! However, this will invalidate neighbor searcher and neighbor lists. It
    //! is users responsibility to call
    //! ParticleSystemData3::buildNeighborSearcher and
    //! ParticleSystemData3::buildNeighborLists to refresh those data.
    //!
    //! \param[in]  newPosition The new position.
    //! \param[in]  newVelocity The new velocity.
    //! \param[in]  newForce    The new force.
    //!
    void addParticle(const Vector3D &newPosition, const Vector3D &newVelocity = Vector3D(), const Vector3D &newForce = Vector3D());

    //!
    //! \brief      Adds particles to the data structure.
    //!
    //! This function will add particles to the data structure. For custom data
    //! layers, zeros will be assigned for new particles. However, this will
    //! invalidate neighbor searcher and neighbor lists. It is users
    //! responsibility to call ParticleSystemData3::buildNeighborSearcher and
    //! ParticleSystemData3::buildNeighborLists to refresh those data.
    //!
    //! \param[in]  newPositions  The new positions.
    //! \param[in]  newVelocities The new velocities.
    //! \param[in]  newForces     The new forces.
    //!
    void addParticles(const ConstArrayAccessor1<Vector3D> &newPositions, const ConstArrayAccessor1<Vector3D> &newVelocities = ConstArrayAccessor1<Vector3D>(), const ConstArrayAccessor1<Vector3D> &newForces = ConstArrayAccessor1<Vector3D>());

    //!
    //! \brief      Returns neighbor searcher.
    //!
    //! This function returns currently set neighbor searcher object. By
    //! default, PointParallelHashGridSearcher3 is used.
    //!
    //! \return     Current neighbor searcher.
    //!
    auto neighborSearcher() const -> const PointNeighborSearcher3Ptr &;

    //! Sets neighbor searcher.
    void setNeighborSearcher(const PointNeighborSearcher3Ptr &newNeighborSearcher);

    //!
    //! \brief      Returns neighbor lists.
    //!
    //! This function returns neighbor lists which is available after calling
    //! PointParallelHashGridSearcher3::buildNeighborLists. Each list stores
    //! indices of the neighbors.
    //!
    //! \return     Neighbor lists.
    //!
    auto neighborLists() const -> const std::vector<std::vector<size_t>> &;

    //! Builds neighbor searcher with given search radius.
    void buildNeighborSearcher(double maxSearchRadius);

    //! Builds neighbor lists with given search radius.
    void buildNeighborLists(double maxSearchRadius);

    //! Serializes this particle system data to the buffer.
    void serialize(std::vector<uint8_t> *buffer) const override;

    //! Deserializes this particle system data from the buffer.
    void deserialize(const std::vector<uint8_t> &buffer) override;

    //! Copies from other particle system data.
    void set(const ParticleSystemData3 &other);

    //! Copies from other particle system data.
    auto operator=(const ParticleSystemData3 &other) -> ParticleSystemData3 &;

protected:
    void serializeParticleSystemData(flatbuffers::FlatBufferBuilder *builder, flatbuffers::Offset<fbs::ParticleSystemData3> *fbsParticleSystemData) const;

    void deserializeParticleSystemData(const fbs::ParticleSystemData3 *fbsParticleSystemData);

private:
    double _radius = 1e-3;
    double _mass = 1e-3;
    size_t _numberOfParticles = 0;
    size_t _positionIdx;
    size_t _velocityIdx;
    size_t _forceIdx;

    std::vector<ScalarData> _scalarDataList;
    std::vector<VectorData> _vectorDataList;

    PointNeighborSearcher3Ptr _neighborSearcher;
    std::vector<std::vector<size_t>> _neighborLists;
};

//! Shared pointer type of ParticleSystemData3.
using ParticleSystemData3Ptr = std::shared_ptr<ParticleSystemData3>;

}  // namespace HinaPE::FluidEngine

#endif  // HINAPE_FLUID_ENGINE_PARTICLE_SYSTEM_DATA3_H_
