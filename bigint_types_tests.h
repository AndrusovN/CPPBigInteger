#pragma once 

#include "bigint_test_helper.h"

TEST(BiConstructorTests, LLTime) {
    int total_time = 0;
    int time_treshold = 1500000;
    for (size_t i = 0; i < 10000; ++i) {
        Timer T;
        T.start();
        for (size_t j = 0; j < 1000; ++j) {
            BigInteger a(1ll);
        }
        T.finish();
        total_time += T.get_time_microseconds();
        ASSERT_LE(total_time, time_treshold) << i << " iteration of 10000";
    }
}

TEST(BiConstructorTests, Default) {
    BigInteger a;

    ASSERT_EQ(0, a);
}

TEST(BiConstructorTests, LLPositive) {
    long long value = 179179179179ll;
    BigInteger a = value;

    ASSERT_EQ(value, a);
}

TEST(BiConstructorTests, LLNegative) {
    long long value = -9876543212345ll;
    BigInteger a = value;

    ASSERT_EQ(value, a);
}

TEST(BiConstructorTests, CopyConstructor) {
    BigInteger a(179);
    BigInteger b = a;

    ASSERT_EQ(a, b);
}

TEST(BiConstructorTests, CopyNegative) {
    BigInteger a(-179);
    BigInteger b = a;

    ASSERT_EQ(a, b);
}

TEST(BiConstructorTests, StringPositive) {
    int num = 1791791791;
    BigInteger a(std::to_string(num));
    
    ASSERT_EQ(num, a);
}

TEST(BiConstructorTests, StringZeroes) {
    BigInteger a("00000");

    ASSERT_EQ(0, a);
}

TEST(BiConstructorTests, StringNegative) {
    BigInteger a("-179");

    ASSERT_EQ(-179, a);
}

TEST(BiConstructorTests, StringZeroesBegin) {
    BigInteger a("000179");

    ASSERT_EQ(179, a);
}

TEST(BiConstructorTests, Assign) {
    BigInteger a = 179;
    BigInteger b = 57;
    b = a;
    
    ASSERT_EQ(a, b);
}

TEST(BiConstructorTests, AssingChangeSign) {
    BigInteger a = 179;
    BigInteger b = -57;
    b = a;

    ASSERT_EQ(a, b);
}

TEST(BiConstructorTests, AssignZero) {
    BigInteger a = 179;
    a = 0;

    ASSERT_EQ(0, a);
}

TEST(BiMethodTests, ToString) {
    BigInteger a = 1791791791;

    ASSERT_EQ("1791791791", a.toString());
}

TEST(BiMethodTests, ToStringFromString) {
    string s = "476321836354832172498732156235748521692374863219823435621786345367821";
    BigInteger a(s);

    ASSERT_EQ(s, a.toString());
}

TEST(BiMethodTests, ToStringZero) {
    BigInteger a = 0;

    ASSERT_EQ("0", a.toString());
}

TEST(BiMethodTests, ToStringMoreZeroes) {
    BigInteger a = 10;
    --a;

    ASSERT_EQ("9", a.toString());
}

TEST(BiMethodTests, ToStringNeg) {
    BigInteger a = -179;

    ASSERT_EQ("-179", a.toString());
}

TEST(BiMethodTests, ToStringZeroesFromString) {
    BigInteger a("000000000179");

    ASSERT_EQ("179", a.toString());
}

TEST(BiOperatorTests, LLCast) {
    BigInteger a = 1791791791;
    long long b = static_cast<long long>(a);

    ASSERT_EQ(a, b);
}

TEST(BiOperatorTests, LLCastNeg) {
    BigInteger a = -179179179179179179ll;
    long long b = static_cast<long long>(a);

    ASSERT_EQ(a, b);
}

TEST(BiOperatorTests, LLCastMemory) {
    BigInteger a = 11;
    OperatorNewCounter cntr;
    static_cast<long long>(a);
    ASSERT_EQ(0, cntr.get_counter());
}

TEST(BiOperatorTests, BoolCast) {
    BigInteger a = 179;

    ASSERT_TRUE(static_cast<bool>(a));
}

TEST(BiOperatorTests, BoolCastNeg) {
    BigInteger a = -179;

    ASSERT_TRUE(static_cast<bool>(a));
}

TEST(BiOperatorTests, BoolFalse) {
    BigInteger a = 0;

    ASSERT_FALSE(static_cast<bool>(a));
}

TEST(BiOperatorTests, Input) {
    std::stringstream testStream;
    testStream << "1791791791";
    BigInteger a;
    testStream >> a;

    ASSERT_EQ(1791791791, a);
}

TEST(BiOperatorTests, InputNeg) {
    std::stringstream testStream;
    testStream << "-1791791791";
    BigInteger a;
    testStream >> a;

    ASSERT_EQ(-1791791791, a);
}

TEST(BiOperatorTests, InputZeroes) {
    std::stringstream testStream;
    testStream << "0001";
    BigInteger a;
    testStream >> a;
    ASSERT_EQ(1, a);
}

TEST(BiOperatorTests, Output) {
    std::stringstream testStream;
    BigInteger a = 1791791791;
    testStream << a;
    int result;
    testStream >> result;
    ASSERT_EQ(a, result);
}

TEST(BiOperatorTests, OutputNegative) {
    std::stringstream ss;
    BigInteger a = -1791791791;
    ss << a;
    int result;
    ss >> result;
    ASSERT_EQ(a, result);
}

TEST(BiOperatorTests, OutputZero) {
    std::stringstream ss;
    BigInteger a = 0;
    ss << a;
    int res;
    ss >> res;
    ASSERT_EQ(a, res);
}

TEST(BiOperatorTests, IORandom) {
    std::stringstream ss;
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        BigInteger a = random_bigint(100);
        ss << a << ' ';
        BigInteger b;
        ss >> b;
        ASSERT_EQ(a, b);
    }
}

TEST(BiOperatorTests, Literal) {
    auto a = 1791791791_bi;
    ASSERT_EQ(1791791791, a);
}

TEST(BiOperatorTests, LiteralZero) {
    auto a = 000000000_bi;
    ASSERT_EQ(0, a);
}

