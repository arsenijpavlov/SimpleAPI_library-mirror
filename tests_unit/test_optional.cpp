#include <SimpleAPI.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}

//========================================================================================

TEST(OPTIONAL, creator) {
    using namespace simpleapi;

    Optional<char> oc('a');
    EXPECT_TRUE(oc.isValid());
    EXPECT_EQ(oc.value(), 'a');
    oc.unset();
    EXPECT_EQ(oc.value(), 'a');

    oc = Optional<char>('b', false);
    EXPECT_TRUE(!oc.isValid());
    EXPECT_EQ(oc.value(), 'b');
}

TEST(OPTIONAL, compare) {
    using namespace simpleapi;

    Optional<int> oi(15);
    EXPECT_EQ(oi, 15);

    Optional<int> oi2(16);
    EXPECT_NE(oi, oi2);
    oi2.set(oi.value());
    EXPECT_EQ(oi, oi2.value());

    oi.unset();
    EXPECT_NE(oi, 15);

}
