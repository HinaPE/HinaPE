#ifndef HINAPE_SERIALIZATION_H_
#define HINAPE_SERIALIZATION_H_

#include "array/array1.h"

#include <vector>

namespace HinaPE
{

//! Abstract base class for any serializable class.
class Serializable
{
public:
    Serializable() = default;

    virtual ~Serializable() = default;

    //! Serializes this instance into the flat buffer.
    virtual void serialize(std::vector<uint8_t> *buffer) const = 0;

    //! Deserializes this instance from the flat buffer.
    virtual void deserialize(const std::vector<uint8_t> &buffer) = 0;
};

//! Serializes serializable object.
void serialize(const Serializable *serializable, std::vector<uint8_t> *buffer);

//! Serializes data chunk using common schema.
void serialize(const uint8_t *data, size_t size, std::vector<uint8_t> *buffer);

//! Serializes data chunk using common schema.
template<typename T>
void serialize(const ConstArrayAccessor1<T> &array, std::vector<uint8_t> *buffer);

//! Deserializes buffer to serializable object.
void deserialize(const std::vector<uint8_t> &buffer, Serializable *serializable);

//! Deserializes buffer to data chunk using common schema.
void deserialize(const std::vector<uint8_t> &buffer, std::vector<uint8_t> *data);

//! Deserializes buffer to data chunk using common schema.
template<typename T>
void deserialize(const std::vector<uint8_t> &buffer, Array1<T> *array);

}  // namespace HinaPE

#include "serialization-inl.h"

#endif  // HINAPE_SERIALIZATION_H_
