#include <gtest/gtest.h>
#include <sstream>
#include <limits>
#include "serializer.hpp"
#include "deserializer.hpp"
#include "data.hpp"

TEST(DeserializerTest, DeserializeSimple) {
    Data x{1, true, 2};
    std::stringstream ss;
    Serializer serializer(ss);
    ASSERT_EQ(serializer.save(x), Error::NoError);

    Data y{0, false, 0};
    Deserializer deserializer(ss);
    EXPECT_EQ(deserializer.load(y), Error::NoError);
    EXPECT_EQ(y.a, x.a);
    EXPECT_EQ(y.b, x.b);
    EXPECT_EQ(y.c, x.c);
}

TEST(DeserializerTest, DeserializeEdgeValues) {
    Data x{0, true, std::numeric_limits<uint64_t>::max()};
    std::stringstream ss;
    Serializer s(ss);
    ASSERT_EQ(s.save(x), Error::NoError);

    Data y{1, false, 1};
    Deserializer d(ss);
    EXPECT_EQ(d.load(y), Error::NoError);
    EXPECT_EQ(y.a, x.a);
    EXPECT_TRUE(y.b);
    EXPECT_EQ(y.c, x.c);
}

TEST(DeserializerTest, CorruptedMissingField) {
    std::stringstream ss("1 true");  // only two fields
    Data y{0, false, 0};
    Deserializer d(ss);
    EXPECT_EQ(d.load(y), Error::CorruptedArchive);
}

TEST(DeserializerTest, CorruptedInvalidBool) {
    std::stringstream ss("1 yes 2");  // «yes» is not valid
    Data y{0, false, 0};
    Deserializer d(ss);
    EXPECT_EQ(d.load(y), Error::CorruptedArchive);
}

TEST(DeserializerTest, CorruptedInvalidNumber) {
    std::stringstream ss("foo true 2");  // «foo» is not a number
    Data y{0, false, 0};
    Deserializer d(ss);
    EXPECT_EQ(d.load(y), Error::CorruptedArchive);
}

TEST(DeserializerTest, IgnoreExtraData) {
    std::stringstream ss("5 false 10 extra data");
    Data y{0, true, 0};
    Deserializer d(ss);
    EXPECT_EQ(d.load(y), Error::NoError);

    // After three fields, there are "extra data" left in the input stream
    std::string leftover1, leftover2;
    ss >> leftover1 >> leftover2;
    EXPECT_EQ(leftover1, "extra");
    EXPECT_EQ(leftover2, "data");
}
