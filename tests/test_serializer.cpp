#include <gtest/gtest.h>
#include <sstream>
#include <limits>
#include "serializer.hpp"
#include "data.hpp"

TEST(SerializerTest, SerializeSimple) {
    Data x{1, true, 2};
    std::stringstream ss;
    Serializer serializer(ss);

    EXPECT_EQ(serializer.save(x), Error::NoError);
    EXPECT_EQ(ss.str(), "1 true 2 ");
}

TEST(SerializerTest, SerializeEdgeValues) {
    Data x{0, false, std::numeric_limits<uint64_t>::max()};
    std::stringstream ss;
    Serializer serializer(ss);

    EXPECT_EQ(serializer.save(x), Error::NoError);
    EXPECT_EQ(ss.str(),
              "0 false " + std::to_string(std::numeric_limits<uint64_t>::max()) + " ");
}

TEST(SerializerTest, SerializeBoolCombinations) {
    // b = false
    {
        Data x{42, false, 99};
        std::stringstream ss;
        Serializer serializer(ss);
        EXPECT_EQ(serializer.save(x), Error::NoError);
        EXPECT_EQ(ss.str(), "42 false 99 ");
    }
    // b = true
    {
        Data x{42, true, 99};
        std::stringstream ss;
        Serializer serializer(ss);
        EXPECT_EQ(serializer.save(x), Error::NoError);
        EXPECT_EQ(ss.str(), "42 true 99 ");
    }
}
