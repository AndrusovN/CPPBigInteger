#include <sstream>
#include <gtest/gtest.h>
#include <random>

#include "biginteger.h"
#include "helper.h"

mt_19937 test_random(1791791791);

long long random_value() {
    return test_random();
}

BigInteger random_bigint(int size) {
    BigInteger value = 1;
    for (int i = 0; i < size / 9; ++i) {
        value *= random_value();
    }
    return value;
}

TEST(ExceptionTests, CheckWhat) {
    try {
        throw InvalidInputException("aboba");
    } catch (std::exception& e) {
        ASSERT_EQ(string("Invalid input to create BigInteger: aboba. Expected a numerical literal"), string(e.what()));
    }
}

TEST(ConstructorTests, BiDefault) {
    BigInteger a;

    ASSERT_EQ(0, a);
}

TEST(ConstructorTests, BiLLPositive) {
    long long value = 179179179179ll;
    BigInteger a = value;

    ASSERT_EQ(value, a);
}

TEST(ConstructorTests, BiLLNegative) {
    long long value = -9876543212345ll;
    BigInteger a = value;

    ASSERT_EQ(value, a);
}

TEST(ConstructorTests, BiULL) {
    unsigned long long value = 888888888888888888ull;
    BigInteger a = value;

    ASSERT_EQ(value, a);
}

TEST(ConstructorTests, BiCopyConstructor) {
    BigInteger a(179);
    BigInteger b = a;

    ASSERT_EQ(a, b);
}

TEST(ConstructorTests, BiCopyNegative) {
    BigInteger a(-179);
    BigInteger b = a;

    ASSERT_EQ(a, b);
}

TEST(ConstructorTests, BiStringPositive) {
    int num = 1791791791;
    BigInteger a = std::to_string(num);
    
    ASSERT_EQ(num, a);
}

TEST(ConstructorTests, BiStringZeroes) {
    BigInteger a = "00000";

    ASSERT_EQ(0, a);
}

TEST(ConstructorTests, BiStringNegative) {
    BigInteger a = "-179";

    ASSERT_EQ(-179, a);
}

TEST(ConstructorTests, BiStringZeroesBegin) {
    BigInteger a = "000179";

    ASSERT_EQ(179, a);
}

TEST(ConstructorTests, BiStringException) {
    ASSERT_ANY_THROW(BigInteger a = "179qwerty");
}

TEST(ConstructorTests, BiAssign) {
    BigInteger a = 179;
    BigInteger b = 57;
    b = a;
    
    ASSERT_EQ(a, b);
}

TEST(ConstructorTests, BiAssingChangeSign) {
    BigInteger a = 179;
    BigInteger b = -57;
    b = a;

    ASSERT_EQ(a, b);
}

TEST(ConstructorTests, BiAssignZero) {
    BigInteger a = 179;
    a = 0;

    ASSERT_EQ(0, a);
}

TEST(OperatorTests, BiPlusEQ) {
    BigInteger a = 179;
    a += 179;
    
    ASSERT_EQ(179l+179l, a);
}

TEST(OperatorTests, BiPlusEQNeg) {
    BigInteger a = -179;
    a += 57;

    ASSERT_EQ(57-179, a);
}

TEST(OperatorTests, BiPlusEQNegNeg) {
    BigInteger a = -179;
    a += -57;

    ASSERT_EQ(-179-57, a);
}

TEST(OperatorTests, BiPlusEQNegPos) {
    BigInteger a = -57;
    a += 179;

    ASSERT_EQ(179-57, a);
}

TEST(OperatorTests, BiPlusEQPosNeg) {
    BigInteger a = 179;
    a += -57;
    ASSERT_EQ(179-57, a);
}

TEST(OperatorTests, BiPlusEQPosNegOverflow) {
    BigInteger a = 57;
    a += -179;

    ASSERT_EQ(57-179, a);
}

TEST(OperatorTests, BiPlusEQRandom) {
    for (auto i = 0; i < 20; ++i) {
        long long first = random_value();
        long long second = random_value();
        BigInteger a = first;
        a += second;
        ASSERT_EQ(first + second, a);
    }
}

TEST(OperatorTests, BiPlusEQTime) {
    int total_time = 0;
    int time_treshold = 1000;
    for (int i = 0; i < 100; ++i) {
        BigInteger first = random_bigint(1e5);
        BigInteger second = random_bigint(1e5);
        Timer T;
        T.start();
        first += second;
        T.end();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(OperatorTests, BiMinusEQ) {
    BigInteger a = 11;
    for (int i = -25; i < 25; ++i) {
        a = 11;
        a -= i;
        ASSERT_EQ(11 - i, a);
    }
}

TEST(OperatorTests, BiMinusEQRandom) {
    for (auto i = 0; i < 20; ++i) {
        long long first = random_value();
        long long second = random_value();
        BigInteger a = first;
        a -= second;
        ASSERT_EQ(first - second, a);
    }
}

TEST(OperatorTests, BiTimesEQ) {
    BigInteger a = 57;
    a *= 4;

    ASSERT_EQ(228, a);
}

TEST(OperatorTests, BiTimesEQNeg) {
    BigInteger a = -57;
    a *= 4;

    ASSERT_EQ(-228, a);
}

TEST(OperatorTests, BiTimesEQNegNeg) {
    BigInteger a = -57;
    a *= -4;

    ASSERT_EQ(228, a);
}

TEST(OperatorTests, BiTimesEQPosNeg) {
    BigInteger a = 4;
    a *= -57;

    ASSERT_EQ(-228, a);
}

TEST(OperatorTests, BiTimesEQZero) {
    BigInteger a = 1791791791;
    a *= 0;

    ASSERT_EQ(0, a);
}

TEST(OperatorTests, BiTimesEQZeroNeg) {
    BigInteger a = -1791791791;
    a *= 0;

    ASSERT_EQ(0, a);
}

TEST(OperatorTests, BiTimesEQRandom) {
    for (auto i = 0; i < 20; ++i) {
        long long first = random_value();
        long long second = random_value();
        BigInteger a = first;
        a *= second;
        ASSERT_EQ(first * second, a);
    }
}

TEST(OperatorTests, BiTimesEQTime) {
    int total_time = 0;
    int time_treshold = 2000;

    for (int i = 0; i < 100; ++i) {
        auto first = random_bigint(1e4);
        auto second = random_bigint(1e4);
        Timer T;
        T.start();
        first *= second;
        T.end();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

void test_division(long long left, long long right) {
    BigInteger a = left;
    a /= right;

    ASSERT_EQ(left / right, a);
}

TEST(OperatorTests, BiDivEQ) {
    test_division(179, 57);
}

TEST(OperatorTests, BiDivNeg) {
    test_division(-179, 57);
}

TEST(OperatorTests, BiDivPosNeg) {
    test_division(-179, 57)
}

TEST(OperatorTests, BiDivNegNeg) {
    test_division(-179, -57);
}

TEST(OperatorTests, BiDivRandom) {
    for (int i = 0; i < 20; ++i) {
        auto left = random_value();
        auto right = 0;
        while (right == 0) right = random_value();
        test_division(left, right);
    }
}

TEST(OperatorTests, BiDivZeroDivisionExc) {
    BigInteger a = 179;
    ASSERT_ANY_THROW(a / 0);
}

TEST(OperatorTests, BiDivTime) {
    int total_time = 0;
    int time_treshold = 2000;

    for (int i = 0; i < 100; ++i) {
        auto first = random_bigint(5e2);
        auto second = random_bigint(5e2);
        Timer T;
        T.start();
        first /= second;
        T.end();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

void test_modulus(long long left, long long right) {
    BigInteger a = left;
    a %= right;
    
    ASSERT_EQ(left % right, a);
}

TEST(OperatorTests, BiMod) {
    test_modulus(179, 57);
}

TEST(OperatorTests, BiModNeg) {
    test_modulus(-179, 57);
}

TEST(OperatorTests, BiModPosNeg) {
    test_modulus(179, -57);
}

TEST(OperatorTests, BiModNegNeg) {
    test_modulus(-179, -179);
}

TEST(OperatorTests, BiModZeroDivisionExc) {
    BigInteger a = 179;
    ASSERT_ANY_THROW(a % 0);
}

TEST(OperatorTests, BiModRandom) {
    for (int i = 0; i < 20; ++i) {
        long long left = random_value();
        long long right = 0;
        while (right == 0) right = random_value();

        test_modulus(left, right);
    }
}

TEST(OperatorTests, BiModTime) {
    int total_time = 0;
    int time_treshold = 2000;

    for (int i = 0; i < 100; ++i) {
        auto first = random_bigint(5e2);
        auto second = random_bigint(5e2);
        Timer T;
        T.start();
        first %= second;
        T.end();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(OperatorTests, BiLPPlus) {
    BigInteger a = 178;
    ASSERT_EQ(179, ++a);
    ASSERT_EQ(179, a);
}

TEST(OperatorTests, BiLPPlusNeg) {
    BigInteger a = -180;
    ASSERT_EQ(-179, ++a);
    ASSERT_EQ(-179, a);
}

TEST(OperatorTests, BiLPPlusNegZero) {
    BigInteger a = -1;
    ASSERT_EQ(0, ++a);
    ASSERT_EQ(0, a);
}

TEST(OperatorTests, BiLPPlusDoublePlus) {
    BigInteger a = 177;
    ASSERT_EQ(179, ++++a);
    ASSERT_EQ(179, a);
}

TEST(OperatorTests, BiLPPlusTime) {
    int total_time = 0;
    int time_treshold = 1000;
    
    for (int i = 0; i < 100; ++i) {
        Timer T;
        BigInteger first = random_bigint(1e5);
        T.start();
        
        for (int j = 0; j < 1e5; ++j) 
            ++first;
        }

        T.end();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(OperatorTests, BiRPPlus) {
    BigInteger a = 178;
    ASSERT_EQ(178, a++);
    ASSERT_EQ(179, a);
}

TEST(OperatorTests, BiRPPlusNeg) {
    BigInteger a = -180;
    ASSERT_EQ(-180, a++);
    ASSERT_EQ(-179, a);
}

TEST(OperatorTests, BiRPPlusNegZero) {
    BigInteger a = -1;
    ASSERT_EQ(-1, a++);
    ASSERT_EQ(0, a);
}

TEST(OperatorTests, BiLMMinus) {
    BigInteger a = 180;
    ASSERT_EQ(179, --a);
    ASSERT_EQ(179, a);
}

TEST(OperatorTests, BiLMMinusNeg) {
    BigInteger a = -178;
    ASSERT_EQ(-179, --a);
    ASSERT_EQ(-179, a);
}

TEST(OperatorTests, BiLMMinusPosZero) {
    BigInteger a = 1;
    ASSERT_EQ(0, --a);
    ASSERT_EQ(0, a);
}

TEST(OperatorTests, BiLMMinusDoubleMinus) {
    BigInteger a = 181;
    ASSERT_EQ(179, ----a);
    ASSERT_EQ(179, a);
}

TEST(OperatorTests, BiLMMinusTime) {
    int total_time = 0;
    int time_treshold = 1000;
    
    for (int i = 0; i < 100; ++i) {
        Timer T;
        BigInteger first = random_bigint(1e5);
        T.start();
        
        for (int j = 0; j < 1e5; ++j) 
            --first;
        }

        T.end();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(OperatorTests, BiRMMinus) {
    BigInteger a = 180;
    ASSERT_EQ(180, a--);
    ASSERT_EQ(179, a);
}

TEST(OperatorTests, BiRMMinusNeg) {
    BigInteger a = -178;
    ASSERT_EQ(-178, a--);
    ASSERT_EQ(-179, a);
}

TEST(OperatorTests, BiRMMinusPosZero) {
    BigInteger a = 1;
    ASSERT_EQ(1, a--);
    ASSERT_EQ(0, a);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

