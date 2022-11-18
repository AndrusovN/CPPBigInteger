#pragma once

#include "bigint_tests_helper.h"

TEST(BiOperatorTests, EqTrue) {
    BigInteger a = 179;
    BigInteger b = 179;

    ASSERT_TRUE(a == b);
}

TEST(BiOperatorTests, EqFalse) {
    BigInteger a = 179;
    BigInteger b = 57;

    ASSERT_FALSE(a == b);
}

TEST(BiOperatorTests, EqSignedNeq) {
    BigInteger a = 179;
    BigInteger b = -179;

    ASSERT_FALSE(a == b);
}

TEST(BiOperatorTests, EqTime) {
    std::vector<BigInteger> testCases;
    for (int i = 0; i < 179; ++i) {
        testCases.push_back(random_bigint());
        testCases.back() -= testCases.back() % 179;
        testCases.back() += i;
    }

    int total_time = 0;
    int time_treshold = 1000000;
    for (int i = 0; i < 179; ++i) {
        for (int j = i + 1; j < 179; ++j) {
            Timer T;
            T.start();
            testCases[i] == testCases[j];
            T.end();
            total_time += T.get_time_microseconds();
            ASSERT_LE(total_time, time_treshold);
        }
    }
}

TEST(BiOperatorTests, EqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(==, 0);
}

TEST(BiOperatorTests, NeqRandom) {
    for (int i = 0; i < 20; ++i) {
        BigInteger first = random_bigint();
        BigInteger second = random_bigint();
        ASSERT_EQ(first == second, !(first != second));
    }
}

TEST(BiOperatorTests, NeqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(!=, 0);
}

TEST(BiOperatorTests, SpaceshipG) {
    BigInteger a = 179;
    BigInteger b = 57;

    auto cmp = a <=> b;
    ASSERT_EQ(std::strong_ordering::greater, cmp);
}

TEST(BiOperatorTests, SpaceshipL) {
    BigInteger a = 57;
    BigInteger b = 179;

    ASSERT_EQ(std::strong_ordering::less, a <=> b);
}

TEST(BiOperatorTests, SpacesipEq) {
    BigInteger a = 179;
    BigInteger b = 179;

    ASSERT_EQ(std::strong_ordering::equivalent, a <=> b);
}

TEST(BiOperatorTests, SpaceshipGNeg) {
    BigInteger a = 57;
    BigInteger b = -179;

    ASSERT_EQ(std::strong_ordering::greated, a <=> b);
}

TEST(BiOperatorTests, SpaceshipLNeg) {
    BigInteger a = -179;
    BigInteger b = 57;

    ASSERT_EQ(std::strong_ordering::less, a <=> b);
}

TEST(BiOperatorTests, SpaceshipTwoNeg) {
    BigInteger a = -179;
    BigInteger b = -57;

    ASSERT_EQ(std::strong_ordering::less, a <=> b);
}

TEST(BiOperatorTests, SpaceshipTwoNegEq) {
    BigInteger a = -179;
    BigInteger b = -179;

    ASSERT_EQ(std::strong_ordering::equivalent, a <=> b);
}

TEST(BiOperatorTests, SpaceshipSign) {
    BigInteger a = 179;
    BigInteger b = -179;

    ASSERT_EQ(std::strong_ordering::greater, a <=> b);
}

TEST(BiOperatorTests, SpaceshipTime) {
    int total_time = 0;
    int time_treshold = 1000000;
    for (int i = 0; i < 1000; ++i) {
        BigInteger a = random_bigint(1e4);
        BigInteger b = random_bigint(1e4 - 30);

        Timer T;
        T.start();
        auto res = a <=> b;
        T.end();
        total_time += T.get_time_microseconds();
        ASSERT_LE(time_treshold, total_time);
    }
}

TEST(BiOperatorTests, SpaceshipMemory) {
    CHECK_OPERATOR_ALLOCATIONS(<=>, 0);
}

TEST(BiOperatorTests, Le) {
    CHECK_AGREED_WITH_INT(<);
}

TEST(BiOperatorTests, Leq) {
    CHECK_AGREED_WITH_INT(<=);
}

TEST(BiOperatorTests, Ge) {
    CHECK_AGREED_WITH_INT(>);
}

TEST(BiOperatorTests, Geq) {
    CHECK_AGREED_WITH_INT(>=);
}

