#pragma once

#include "bigint_test_helper.h"

TEST(BiOperatorTests, PlusRandom) {
    TEST_SAME_OPERATOR(+, +=);
}

TEST(BiOperatorTests, PlusMemory) {
    CHECK_OPERATOR_ALLOCATIONS(+, 1)
}

TEST(BiOperatorTests, MinusRandom) {
    TEST_SAME_OPERATOR(-, -=);
}

TEST(BiOperatorTests, MinusMemory) {
    CHECK_OPERATOR_ALLOCATIONS(-, 1);
}

TEST(BiOperatorTests, MultRandom) {
    TEST_SAME_OPERATOR(*, *=);
}

TEST(BiOperatorTests, MultMemory) {
    CHECK_OPERATOR_ALLOCATIONS(*, 3);
}

TEST(BiOperatorTests, DivRandom) {
    TEST_SAME_OPERATOR(/, /=);
}

TEST(BiOperatorTests, DivMemory) {
    CHECK_OPERATOR_ALLOCATIONS(/, 2);
}

TEST(BiOperatorTests, ModRandom) {
    TEST_SAME_OPERATOR(%, %=);
}

TEST(BiOperatorTests, ModMemory) {
    CHECK_OPERATOR_ALLOCATIONS(%, 4);
}

TEST(BiOperatorTests, PlusEQ) {
    BigInteger a = 179;
    a += 179;
    
    ASSERT_EQ(179+179, a);
}

TEST(BiOperatorTests, PlusEQNeg) {
    BigInteger a = -179;
    a += 57;

    ASSERT_EQ(57-179, a);
}

TEST(BiOperatorTests, PlusEQNegNeg) {
    BigInteger a = -179;
    a += -57;

    ASSERT_EQ(-179-57, a);
}

TEST(BiOperatorTests, PlusEQNegPos) {
    BigInteger a = -57;
    a += 179;

    ASSERT_EQ(179-57, a);
}

TEST(BiOperatorTests, PlusEQPosNeg) {
    BigInteger a = 179;
    a += -57;
    ASSERT_EQ(179-57, a);
}

TEST(BiOperatorTests, PlusEQPosNegOverflow) {
    BigInteger a = 57;
    a += -179;

    ASSERT_EQ(57-179, a);
}

TEST(BiOperatorTests, PlusEQRandom) {
    for (auto i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        long long first = random_value();
        long long second = random_value();
        BigInteger a = first;
        a += second;
        ASSERT_EQ(first + second, a);
    }
}

TEST(BiOperatorTests, PlusEQTime) {
    int total_time = 0;
    int time_treshold = 1000000;
    for (int i = 0; i < 2000; ++i) {
        BigInteger first = random_bigint(1e4);
        BigInteger second = random_bigint(1e4);
        Timer T;
        T.start();
        first += second;
        T.finish();
        total_time += T.get_time_microseconds();
        ASSERT_LE(total_time, time_treshold) << i << " iteration of 1000";
    }
}

TEST(BiOperatorTests, PlusEqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(+=, 1);
}

TEST(BiOperatorTests, MinusEQ) {
    BigInteger a = 11;
    for (int i = -25; i < 25; ++i) {
        a = 11;
        a -= i;
        ASSERT_EQ(11 - i, a);
    }
}

TEST(BiOperatorTests, MinusEQRandom) {
    for (auto i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        long long first = random_value();
        long long second = random_value();
        BigInteger a = first;
        a -= second;
        ASSERT_EQ(first - second, a);
    }
}

TEST(BiOperatorTests, MinusEqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(-=, 1);
}

void test_mult(long long a, long long b) {
    BigInteger bi = a;
    bi *= b;

    ASSERT_EQ(a * b, bi);
}

TEST(BiOperatorTests, TimesEQ) {
    test_mult(57, 4);
}

TEST(BiOperatorTests, TimesEQNeg) {
    test_mult(-57, 4);
}

TEST(BiOperatorTests, TimesEQNegNeg) {
    test_mult(-57, -4);
}

TEST(BiOperatorTests, TimesEQPosNeg) {
    test_mult(57, -4);
}

TEST(BiOperatorTests, TimesEQZero) {
    test_mult(1791791791, 0);
}

TEST(BiOperatorTests, TimesEQZeroNeg) {
    test_mult(-1791791791, 0);
}

TEST(BiOperatorTests, TimesEQRandom) {
    for (auto i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        test_mult(random_value(), random_value());
    }
}

TEST(BiOperatorTests, TimesEQTime) {
    int total_time = 0;
    int time_treshold = 3000;

    for (int i = 0; i < 100; ++i) {
        auto first = random_bigint(1e4);
        auto second = random_bigint(1e4);
        Timer T;
        T.start();
        first *= second;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold) << i << " of 100 iterations";
    }
}

TEST(BiOperatorTests, TimesEqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(*=, 2);
}

void test_division(long long left, long long right) {
    BigInteger a = left;
    a /= right;

    ASSERT_EQ(left / right, a);
}

TEST(BiOperatorTests, DivEQ) {
    test_division(179, 57);
}

TEST(BiOperatorTests, DivNeg) {
    test_division(-179, 57);
}

TEST(BiOperatorTests, DivPosNeg) {
    test_division(-179, 57);
}

TEST(BiOperatorTests, DivNegNeg) {
    test_division(-179, -57);
}

TEST(BiOperatorTests, DivEqRandom) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        auto left = random_value();
        auto right = 0;
        while (right == 0) right = random_value();
        test_division(left, right);
    }
}

TEST(BiOperatorTests, DivTime) {
    int total_time = 0;
    int time_treshold = 2000;

    for (int i = 0; i < 100; ++i) {
        auto first = random_bigint(5e2);
        auto second = random_bigint(5e2);
        Timer T;
        T.start();
        first /= second;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(BiOperatorTests, DivEqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(/=, 1);
}

void test_modulus(long long left, long long right) {
    BigInteger a = left;
    a %= right;
    
    ASSERT_EQ(left % right, a);
}

TEST(BiOperatorTests, Mod) {
    test_modulus(179, 57);
}

TEST(BiOperatorTests, ModNeg) {
    test_modulus(-179, 57);
}

TEST(BiOperatorTests, ModPosNeg) {
    test_modulus(179, -57);
}

TEST(BiOperatorTests, ModNegNeg) {
    test_modulus(-179, -179);
}

TEST(BiOperatorTests, ModEqRandom) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        long long left = random_value();
        long long right = 0;
        while (right == 0) right = random_value();

        test_modulus(left, right);
    }
}

TEST(BiOperatorTests, ModEqTime) {
    int total_time = 0;
    int time_treshold = 2000;

    for (int i = 0; i < 100; ++i) {
        auto first = random_bigint(5e2);
        auto second = random_bigint(5e2);
        Timer T;
        T.start();
        first %= second;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(BiOperatorTests, ModEqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(%=, 3);
}

TEST(BiOperatorTests, LPPlus) {
    BigInteger a = 178;
    ASSERT_EQ(179, ++a);
    ASSERT_EQ(179, a);
}

TEST(BiOperatorTests, LPPlusNeg) {
    BigInteger a = -180;
    ASSERT_EQ(-179, ++a);
    ASSERT_EQ(-179, a);
}

TEST(BiOperatorTests, LPPlusNegZero) {
    BigInteger a = -1;
    ASSERT_EQ(0, ++a);
    ASSERT_EQ(0, a);
}

TEST(BiOperatorTests, LPPlusDoublePlus) {
    BigInteger a = 177;
    ASSERT_EQ(179, ++++a);
    ASSERT_EQ(179, a);
}

TEST(BiOperatorTests, LPPlusTime) {
    int total_time = 0;
    int time_treshold = 1500000;
    
    for (int i = 0; i < 10000; ++i) {
        Timer T;
        BigInteger first = random_bigint(9);
        T.start();
        
        for (int j = 0; j < 1e3; ++j) {
            ++first;
        }

        T.finish();
        total_time += T.get_time_microseconds();
        ASSERT_LE(total_time, time_treshold) << i << " of 10000 iterations";
    }
}

TEST(BiOperatorTests, LPPlusMemory) {
    BigInteger a = 179;
    OperatorNewCounter cntr;
    ++a;
    ASSERT_EQ(0, cntr.get_counter());
}

TEST(BiOperatorTests, RPPlus) {
    BigInteger a = 178;
    ASSERT_EQ(178, a++);
    ASSERT_EQ(179, a);
}

TEST(BiOperatorTests, RPPlusNeg) {
    BigInteger a = -180;
    ASSERT_EQ(-180, a++);
    ASSERT_EQ(-179, a);
}

TEST(BiOperatorTests, RPPlusNegZero) {
    BigInteger a = -1;
    ASSERT_EQ(-1, a++);
    ASSERT_EQ(0, a);
}

TEST(BiOperatorTests, RPPlusMemory) {
    BigInteger a = 179;
    OperatorNewCounter cntr;
    BigInteger b = a++;
    ASSERT_LE(cntr.get_counter(), 1);
}

TEST(BiOperatorTests, LMMinus) {
    BigInteger a = 180;
    ASSERT_EQ(179, --a);
    ASSERT_EQ(179, a);
}

TEST(BiOperatorTests, LMMinusNeg) {
    BigInteger a = -178;
    ASSERT_EQ(-179, --a);
    ASSERT_EQ(-179, a);
}

TEST(BiOperatorTests, LMMinusPosZero) {
    BigInteger a = 1;
    ASSERT_EQ(0, --a);
    ASSERT_EQ(0, a);
}

TEST(BiOperatorTests, LMMinusDoubleMinus) {
    BigInteger a = 181;
    ASSERT_EQ(179, ----a);
    ASSERT_EQ(179, a);
}

TEST(BiOperatorTests, LMMinusTime) {
    int total_time = 0;
    int time_treshold = 1500;
    
    for (int i = 0; i < 100; ++i) {
        Timer T;
        BigInteger first = random_bigint(1e2);
        T.start();
        
        for (int j = 0; j < 1e5; ++j) {
            --first;
        }

        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold) << i << " of 100 iterations";
    }
}

TEST(BiOperatorTests, LMMinusMemory) {
    BigInteger a = 180;
    OperatorNewCounter cntr;
    --a;
    ASSERT_EQ(cntr.get_counter(), 0);
}

TEST(BiOperatorTests, RMMinus) {
    BigInteger a = 180;
    ASSERT_EQ(180, a--);
    ASSERT_EQ(179, a);
}

TEST(BiOperatorTests, RMMinusNeg) {
    BigInteger a = -178;
    ASSERT_EQ(-178, a--);
    ASSERT_EQ(-179, a);
}

TEST(BiOperatorTests, RMMinusPosZero) {
    BigInteger a = 1;
    ASSERT_EQ(1, a--);
    ASSERT_EQ(0, a);
}

TEST(BiOperatorTests, RMMinusMemory) {
    BigInteger a = 180;
    OperatorNewCounter cntr;
    auto b = a--;
    ASSERT_LE(cntr.get_counter(), 1);
}

TEST(BiMethodsTests, Power) {
    BigInteger a = BigInteger::power(2, 6);
    ASSERT_EQ(a, 64);
}

TEST(BiMethodsTests, PowerZero) {
    BigInteger a = BigInteger::power(2, 0);
    ASSERT_EQ(1, a);
}

TEST(BiMethodsTests, PowerOne) {
    BigInteger a = BigInteger::power(1, 1791791791);
    ASSERT_EQ(1, a);
}

TEST(BiMethodsTests, NegPower) {
    BigInteger a = BigInteger::power(-2, 5);
    ASSERT_EQ(-32, a);
}

TEST(BiMethodsTests, NegEvenPower) {
    BigInteger a = BigInteger::power(-2, 8);
    ASSERT_EQ(256, a);
}

TEST(BiMethodsTests, ZeroPower) {
    BigInteger a = BigInteger::power(0, 1791791791);
    ASSERT_EQ(0, a);
}

TEST(BiMethodsTests, PowerTime) {
    int time_treshold = 2500;
    int total_time = 0;

    for (size_t i = 0; i < 100; ++i) {
        Timer T;
        T.start();
        BigInteger::power(random_bigint(3), random_bigint(3));
        T.finish();
        total_time += T.get_time_milliseconds();

        ASSERT_LE(total_time, time_treshold) << i << " iteration of 100";
    }
}

TEST(BiMethodsTests, Size) {
    BigInteger a = 179;

    ASSERT_EQ(3, a.size());
}

TEST(BiMethodsTests, SizeNegative) {
    BigInteger a = -19;
    ASSERT_EQ(2, a.size());
}

TEST(BiMethodsTests, SizeBig) {
    BigInteger a = random_bigint(100'000);
    ASSERT_EQ(100'000, a.size());
}

TEST(BiMethodsTests, SizeZero) {
    BigInteger a = 0;
    ASSERT_EQ(1, a.size());
}

TEST(BiMethodsTests, IsZeroPositive) {
    BigInteger a = 179;
    ASSERT_FALSE(a.is_zero());
}

TEST(BiMethodsTests, IsZeroNegative) {
    BigInteger a = -179;
    ASSERT_FALSE(a.is_zero());
}

TEST(BiMethodsTests, IsZeroZero) {
    BigInteger a = 0;
    ASSERT_TRUE(a.is_zero());
}

TEST(BiMethodsTests, IsZeroTen) {
    BigInteger a = 10;
    ASSERT_FALSE(a.is_zero());
}

TEST(BiMethodsTests, IsNegativePositive) {
    BigInteger a = 179;
    ASSERT_FALSE(a.is_negative());
}

TEST(BiMethodsTests, IsNegativeZero) {
    BigInteger a = 0;
    ASSERT_FALSE(a.is_negative());
}

TEST(BiMethodsTests, IsNegativeNegative) {
    BigInteger a = -1;
    ASSERT_TRUE(a.is_negative());
}

TEST(BiMethodsTests, IsNegativeAfterSum) {
    BigInteger a = -11;
    a += 11;
    ASSERT_FALSE(a.is_negative());
}

TEST(BiMethodsTests, InvertSignPositive) {
    BigInteger a = 179;
    a.invert_sign();
    ASSERT_EQ(-179, a);
}

TEST(BiMethodsTests, InvertSignNegative) {
    BigInteger a = -179;
    a.invert_sign();
    ASSERT_EQ(179, a);
}

TEST(BiMethodsTests, InvertSignZero) {
    BigInteger a = 0;
    a.invert_sign();
    ASSERT_EQ(0, a);
    ASSERT_FALSE(a.is_negative());
}

TEST(BiMethodsTests, Shift) {
    BigInteger a = 179;
    a.shift(1);
    ASSERT_EQ(1790, a);
}

TEST(BiMethodsTests, ShiftZero) {
    BigInteger a = 179;
    a.shift(0);
    ASSERT_EQ(179, a);
}

TEST(BiMethodsTests, ShiftNegative) {
    BigInteger a = -179;
    a.shift(2);
    ASSERT_EQ(-17900, a);
}

TEST(BiMethodsTests, ShiftZeroValue) {
    BigInteger a = 0;
    a.shift(30);
    ASSERT_EQ(0, a);
    ASSERT_EQ("0", a.toString());
}

TEST(BiMethodsTests, ShiftTime) {
    int total_time = 0;
    int time_treshold = 1000;
    for (size_t i = 0; i < 100; ++i) {
        BigInteger a = random_bigint(100'000);
        Timer T;
        T.start();
        a.shift(100'000);
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

