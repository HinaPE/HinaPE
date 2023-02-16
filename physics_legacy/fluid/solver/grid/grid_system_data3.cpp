#ifdef _MSC_VER
#pragma warning(disable: 4244)
#endif

#include <pch.h>

#include <factory.h>
#include <fbs_helpers.h>
#include <generated/grid_system_data3_generated.h>

#include "grid_system_data3.h"

#include <flatbuffers/flatbuffers.h>

#include <algorithm>
#include <vector>

using namespace HinaPE;
using namespace HinaPE::Fluid;

GridSystemData3::GridSystemData3()
        : GridSystemData3({0, 0, 0}, {1, 1, 1}, {0, 0, 0}) {
}

GridSystemData3::GridSystemData3(
        const HinaPE::Size3& resolution,
        const HinaPE::Vector3D& gridSpacing,
        const HinaPE::Vector3D& origin) {
    _velocity = std::make_shared<HinaPE::FaceCenteredGrid3>();
    _advectableVectorDataList.push_back(_velocity);
    _velocityIdx = 0;
    resize(resolution, gridSpacing, origin);
}

GridSystemData3::GridSystemData3(const GridSystemData3& other) {
    resize(other._resolution, other._gridSpacing, other._origin);

    for (auto& data : other._scalarDataList) {
        _scalarDataList.push_back(data->clone());
    }
    for (auto& data : other._vectorDataList) {
        _vectorDataList.push_back(data->clone());
    }
    for (auto& data : other._advectableScalarDataList) {
        _advectableScalarDataList.push_back(data->clone());
    }
    for (auto& data : other._advectableVectorDataList) {
        _advectableVectorDataList.push_back(data->clone());
    }

    JET_ASSERT(_advectableVectorDataList.size() > 0);

    _velocity = std::dynamic_pointer_cast<HinaPE::FaceCenteredGrid3>(
            _advectableVectorDataList[0]);

    JET_ASSERT(_velocity != nullptr);

    _velocityIdx = 0;
}

GridSystemData3::~GridSystemData3() {
}

void GridSystemData3::resize(
        const HinaPE::Size3& resolution,
        const HinaPE::Vector3D& gridSpacing,
        const HinaPE::Vector3D& origin) {
    _resolution = resolution;
    _gridSpacing = gridSpacing;
    _origin = origin;

    for (auto& data : _scalarDataList) {
        data->resize(resolution, gridSpacing, origin);
    }
    for (auto& data : _vectorDataList) {
        data->resize(resolution, gridSpacing, origin);
    }
    for (auto& data : _advectableScalarDataList) {
        data->resize(resolution, gridSpacing, origin);
    }
    for (auto& data : _advectableVectorDataList) {
        data->resize(resolution, gridSpacing, origin);
    }
}

HinaPE::Size3 GridSystemData3::resolution() const {
    return _resolution;
}

HinaPE::Vector3D GridSystemData3::gridSpacing() const {
    return _gridSpacing;
}

HinaPE::Vector3D GridSystemData3::origin() const {
    return _origin;
}

HinaPE::BoundingBox3D GridSystemData3::boundingBox() const {
    return _velocity->boundingBox();
}

size_t GridSystemData3::addScalarData(
        const HinaPE::ScalarGridBuilder3Ptr& builder,
        double initialVal) {
    size_t attrIdx = _scalarDataList.size();
    _scalarDataList.push_back(
            builder->build(resolution(), gridSpacing(), origin(), initialVal));
    return attrIdx;
}

size_t GridSystemData3::addVectorData(
        const HinaPE::VectorGridBuilder3Ptr& builder,
        const HinaPE::Vector3D& initialVal) {
    size_t attrIdx = _vectorDataList.size();
    _vectorDataList.push_back(
            builder->build(resolution(), gridSpacing(), origin(), initialVal));
    return attrIdx;
}

size_t GridSystemData3::addAdvectableScalarData(
        const HinaPE::ScalarGridBuilder3Ptr& builder,
        double initialVal) {
    size_t attrIdx = _advectableScalarDataList.size();
    _advectableScalarDataList.push_back(
            builder->build(resolution(), gridSpacing(), origin(), initialVal));
    return attrIdx;
}

size_t GridSystemData3::addAdvectableVectorData(
        const HinaPE::VectorGridBuilder3Ptr& builder,
        const HinaPE::Vector3D& initialVal) {
    size_t attrIdx = _advectableVectorDataList.size();
    _advectableVectorDataList.push_back(
            builder->build(resolution(), gridSpacing(), origin(), initialVal));
    return attrIdx;
}

const HinaPE::FaceCenteredGrid3Ptr& GridSystemData3::velocity() const {
    return _velocity;
}

size_t GridSystemData3::velocityIndex() const {
    return _velocityIdx;
}

const HinaPE::ScalarGrid3Ptr& GridSystemData3::scalarDataAt(size_t idx) const {
    return _scalarDataList[idx];
}

const HinaPE::VectorGrid3Ptr& GridSystemData3::vectorDataAt(size_t idx) const {
    return _vectorDataList[idx];
}

const HinaPE::ScalarGrid3Ptr&
GridSystemData3::advectableScalarDataAt(size_t idx) const {
    return _advectableScalarDataList[idx];
}

const HinaPE::VectorGrid3Ptr&
GridSystemData3::advectableVectorDataAt(size_t idx) const {
    return _advectableVectorDataList[idx];
}

size_t GridSystemData3::numberOfScalarData() const {
    return _scalarDataList.size();
}

size_t GridSystemData3::numberOfVectorData() const {
    return _vectorDataList.size();
}

size_t GridSystemData3::numberOfAdvectableScalarData() const {
    return _advectableScalarDataList.size();
}

size_t GridSystemData3::numberOfAdvectableVectorData() const {
    return _advectableVectorDataList.size();
}

void GridSystemData3::serialize(std::vector<uint8_t>* buffer) const {
    flatbuffers::FlatBufferBuilder builder(1024);

    auto resolution = jetToFbs(_resolution);
    auto gridSpacing = jetToFbs(_gridSpacing);
    auto origin = jetToFbs(_origin);

    std::vector<flatbuffers::Offset<HinaPE::fbs::ScalarGridSerialized3>> scalarDataList;
    std::vector<flatbuffers::Offset<HinaPE::fbs::VectorGridSerialized3>> vectorDataList;
    std::vector<flatbuffers::Offset<HinaPE::fbs::ScalarGridSerialized3>>
            advScalarDataList;
    std::vector<flatbuffers::Offset<HinaPE::fbs::VectorGridSerialized3>>
            advVectorDataList;

    serializeGrid(
            &builder,
            _scalarDataList,
            HinaPE::fbs::CreateScalarGridSerialized3,
            &scalarDataList);
    serializeGrid(
            &builder,
            _vectorDataList,
            HinaPE::fbs::CreateVectorGridSerialized3,
            &vectorDataList);
    serializeGrid(
            &builder,
            _advectableScalarDataList,
            HinaPE::fbs::CreateScalarGridSerialized3,
            &advScalarDataList);
    serializeGrid(
            &builder,
            _advectableVectorDataList,
            HinaPE::fbs::CreateVectorGridSerialized3,
            &advVectorDataList);

    auto gsd = HinaPE::fbs::CreateGridSystemData3(
            builder,
            &resolution,
            &gridSpacing,
            &origin,
            _velocityIdx,
            builder.CreateVector(scalarDataList),
            builder.CreateVector(vectorDataList),
            builder.CreateVector(advScalarDataList),
            builder.CreateVector(advVectorDataList));

    builder.Finish(gsd);

    uint8_t *buf = builder.GetBufferPointer();
    size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void GridSystemData3::deserialize(const std::vector<uint8_t>& buffer) {
    auto gsd = HinaPE::fbs::GetGridSystemData3(buffer.data());

    resize(
            HinaPE::fbsToJet(*gsd->resolution()),
            HinaPE::fbsToJet(*gsd->gridSpacing()),
            HinaPE::fbsToJet(*gsd->origin()));

    _scalarDataList.clear();
    _vectorDataList.clear();
    _advectableScalarDataList.clear();
    _advectableVectorDataList.clear();

    deserializeGrid(
            gsd->scalarData(),
            HinaPE::Fluid::Factory::buildScalarGrid3,
            &_scalarDataList);
    deserializeGrid(
            gsd->vectorData(),
            HinaPE::Fluid::Factory::buildVectorGrid3,
            &_vectorDataList);
    deserializeGrid(
            gsd->advectableScalarData(),
            HinaPE::Fluid::Factory::buildScalarGrid3,
            &_advectableScalarDataList);
    deserializeGrid(
            gsd->advectableVectorData(),
            HinaPE::Fluid::Factory::buildVectorGrid3,
            &_advectableVectorDataList);

    _velocityIdx = static_cast<size_t>(gsd->velocityIdx());
    _velocity = std::dynamic_pointer_cast<HinaPE::FaceCenteredGrid3>(
            _advectableVectorDataList[_velocityIdx]);
}
