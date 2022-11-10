#include <sstream>
#include <gtest/gtest.h>

#include "biginteger.h"
#include "helper.h"

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

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

