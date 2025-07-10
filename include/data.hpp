#pragma once

#include <cstdint>
#include "serializer.hpp"
#include "deserializer.hpp"

struct Data
{
    uint64_t a;
    bool     b;
    uint64_t c;

    template <class SerializerT>
    Error serialize(SerializerT& serializer)
    {
        // For Serializer, it will write a, b, c to the stream
        // For Deserializer, it will read a, b, c from the stream
        return serializer(a, b, c);
    }
};
