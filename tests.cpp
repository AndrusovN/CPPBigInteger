#include <compare>
#include <sstream>
#include <gtest/gtest.h>
#include <random>

#include "biginteger.h"
#include "helper.h"

#include "other_tests.h"

TEST(CringeTests, Cringe0) {
    BigInteger bigint = 0;
    ASSERT_EQ(++bigint, 1);
    bigint = -1;
    ASSERT_EQ(++bigint, 0);
}

TEST(CringeTests, Cringe1) {
    BigInteger bigint = 0;       
}

TEST(CringeTests, Cringe2) {
    BigInteger bigint = 0;       
    ASSERT_TRUE((--bigint) == -1);
}

TEST(CringeTests, Cringe3) {
    BigInteger bigint = 0;       
    ASSERT_TRUE((--bigint) == -1);
    ASSERT_TRUE((bigint--) == -1);
}

TEST(CringeTests, Cringe4) {
    BigInteger bigint = 0;       
    ASSERT_TRUE((--bigint) == -1);
    ASSERT_TRUE((bigint--) == -1);
    ASSERT_TRUE(bigint == -2);
}

TEST(CringeTests, Cringe5) {
    BigInteger bigint = 0;       
    ASSERT_TRUE((--bigint) == -1);
    ASSERT_TRUE((bigint--) == -1);
    ASSERT_TRUE(bigint == -2);
    ASSERT_TRUE((++bigint) == -1);
}

TEST(CringeTests, Cringe6) {
    BigInteger bigint = 0;       
    ASSERT_TRUE((--bigint) == -1);
    ASSERT_TRUE((bigint--) == -1);
    ASSERT_TRUE(bigint == -2);
    ASSERT_TRUE((++bigint) == -1);
    ASSERT_TRUE((bigint++) == -1);
}

TEST(CringeTests, Cringe7) {
    BigInteger bigint = 0;       
    ASSERT_TRUE((--bigint) == -1);
    ASSERT_TRUE((bigint--) == -1);
    ASSERT_TRUE(bigint == -2);
    ASSERT_TRUE((++bigint) == -1);
    ASSERT_TRUE((bigint++) == -1);
    ASSERT_TRUE(bigint == 0);
}

TEST(CringeTests, Cringe8) {
    BigInteger bigint = 0;       
    ASSERT_TRUE((--bigint) == -1);
    ASSERT_TRUE((bigint--) == -1);
    ASSERT_TRUE(bigint == -2);
    ASSERT_TRUE((++bigint) == -1);
    ASSERT_TRUE((bigint++) == -1);
    ASSERT_TRUE(bigint == 0);
    ASSERT_TRUE(-bigint == bigint);
}

TEST(CringeTests, Cringe9) {
    BigInteger bigint = 0;       
    ASSERT_TRUE((--bigint) == -1);
    ASSERT_TRUE((bigint--) == -1);
    ASSERT_TRUE(bigint == -2);
    ASSERT_TRUE((++bigint) == -1);
    ASSERT_TRUE((bigint++) == -1);
    ASSERT_TRUE(bigint == 0);
    ASSERT_TRUE(-bigint == bigint);
    bigint = 1;
}

TEST(CringeTests, Cringe10) {
    BigInteger bigint = 0;       
    ASSERT_TRUE((--bigint) == -1);
    ASSERT_TRUE((bigint--) == -1);
    ASSERT_TRUE(bigint == -2);
    ASSERT_TRUE((++bigint) == -1);
    ASSERT_TRUE((bigint++) == -1);
    ASSERT_TRUE(bigint == 0);
    ASSERT_TRUE(-bigint == bigint);
    bigint = 1;
    ASSERT_TRUE(-bigint != bigint);
    ASSERT_TRUE(-bigint == -1);
}

#include "bigint_test_helper.h"
#include "bigint_arithmetics_tests.h"
#include "bigint_types_tests.h"
#include "bigint_equalities_tests.h"
#include "rational_tests.h"



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

