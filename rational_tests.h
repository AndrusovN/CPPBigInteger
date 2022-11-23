#include <gtest/gtest.h>
#include <sstream>
#include <random>

#include "bigint_test_helper.h"
#include "rational.h"

double precision = 1e-14;

Rational random_rational(size_t size) {
    Rational a = random_bigint(size);
    BigInteger b = 0;
    while(b == 0_bi) b = random_bigint(size);
    a /= b;
    return a;
}

TEST(RatConstructorTests, Default) {
    Rational a;
    ASSERT_EQ(0, a);
}

TEST(RatConstructorTests, LLConstructor) {
    Rational a = 1791791791;
    ASSERT_EQ(1791791791, a);
}

TEST(RatConstructorTests, BIConstructor) {
    BigInteger a = 1791791791;
    Rational b = a;
    ASSERT_EQ(a, b);
    ASSERT_EQ(1791791791, a);
}

TEST(RatOperatorTests, PlusEqNatural) {
    Rational a = 1;
    a += 2;
    ASSERT_EQ(3, a);
}

TEST(RatOperatorTests, PlusEq) {
    Rational a = 1;
    a /= 2;
    Rational b = 1;
    b /= 3;
    a += b;
    Rational c = 5;
    c /= 6;
    ASSERT_EQ(c, a);
}

TEST(RatOperatorTests, PlusEqReduction) {
    Rational a = 1;
    a /= 3;
    Rational b = 1;
    b /= 6;
    a += b;
    Rational c = 1;
    c /= 2;
    ASSERT_EQ(a, c);
}

TEST(RatOperatorTests, PlusEqNeg) {
    Rational a = 1;
    a /= 2;
    Rational b = -1;
    b /= 3;
    a += b;
    Rational c = 1;
    c /= 6;
    ASSERT_EQ(c, a);
}

TEST(RatOperatorTests, PlusEqNegToNeg) {
    Rational a = 1;
    a /= 3;
    Rational b = -1;
    b /= 2;
    a += b;
    Rational c = -1;
    c /= 6;
    ASSERT_EQ(c, a);
}

TEST(RatOperatorTests, PlusEqNegWithNeg) {
    Rational a = -1;
    a /= 6;
    Rational b = -1;
    b /= 3;
    a += b;
    Rational c = -1;
    c /= 2;
    ASSERT_EQ(c, a);
}

TEST(RatOperatorTests, PlusEqNegToPos) {
    Rational a = -1;
    a /= 2;
    Rational b = 1;
    a += b;
    Rational c = 1;
    c /= 2;
    ASSERT_EQ(c, a);
}

TEST(RatOperatorTests, PlusEqNegToNeg2) {
    Rational a = -1;
    a /= 3;
    Rational b = 1;
    b /= 6;
    a += b;
    Rational c = -1;
    c /= 6;
    ASSERT_EQ(c, a);
}

TEST(RatOperatorTests, PlusEqToZero) {
    Rational a = 2;
    a /= 4;
    Rational b = -1;
    b /= 2;
    a += b;
    ASSERT_EQ(0, a);
}

TEST(RatOperatorTests, PlusEqTime) {
    int total_time = 0;
    int time_treshold = 1000;
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        auto a = random_rational(100000);
        auto b = random_rational(100000);
        Timer T;
        T.start();
        a += b;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(RatOperatorTests, MinusEqRandom) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        auto a = random_rational(1000);
        auto b = random_rational(1000);
        auto c = a;
        c += -b;
        a -= b;
        ASSERT_EQ(c, a);
    }
}

TEST(RatOperatorTests, MultEqToNatural) {
    Rational a = 1;
    a /= 9;
    a *= 9;
    ASSERT_EQ(1, a);
}

TEST(RatOperatorTests, MultEq) {
    Rational a = 1;
    a /= 6;
    a *= 2;
    Rational c = 1;
    c /= 3;
    ASSERT_EQ(c, a);
}

TEST(RatOperatorTests, MultEqNegative) {
     Rational a = -1;
     a /= 15;
     Rational b = 10;
     b /= 7;
     a *= b;
     Rational c = 2;
     c /= 21;
     ASSERT_EQ(c, a);
}

TEST(RatOperatorTests, MultEqNegNeg) {
    Rational a = -1;
    Rational b = -1;
    a *= b;
    ASSERT_EQ(1, a);
}

TEST(RatOperatorTests, MultEqZeroZero) {
    Rational a = 0;
    Rational b = 0;
    a *= b;
    ASSERT_EQ(0, a);
}

TEST(RatOperatorTests, MultEqZero) {
    Rational a = 0;
    Rational b = 1;
    a *= b;
    ASSERT_EQ(0, a);
}

TEST(RatOperatorTests, MultEqTime) {
    int total_time = 0;
    int time_treshold = 1000;
    for (int i = 0; i < 100; ++i) {
        auto a = random_rational(10000);
        auto b = random_rational(10000);
        Timer T;
        T.start();
        a *= b;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(time_treshold, total_time);
    }
}

TEST(RatOperatorTests, DivEq) {
    Rational a = 1;
    a /= 2;
    a += a;
    ASSERT_EQ(1, a);
}

TEST(RatOperatorTests, DivEqNegative) {
    Rational a = 1;
    a /= -1;
    a += a;
    ASSERT_EQ(-1, a);
}

TEST(RatOperatorTests, DivEqRational) {
    Rational a = 1;
    Rational b = 1;
    a /= 3;
    b /= 5;
    a /= b;
    Rational c = 5;
    c /= 3;
    ASSERT_EQ(c, a);
}

TEST(RatOperatorTests, DivEqZeroException) {
    Rational a = 1;
    ASSERT_THROW(a /= 0, DivisionByZeroException);
}

TEST(RatOperatorTests, DivEqTime) {
    int time_treshold = 1000;
    int total_time = 0;
    for (int i = 0; i < 100; ++i) {
        auto a = random_rational(10000);
        Rational b = 0;
        while(b == 0) b = random_rational(10000);
        Timer T;
        T.start();
        a /= b;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(RatOperatorTests, UnMinus) {
    Rational a = 179;
    a /= 57;
    Rational b = -a;
    ASSERT_EQ(0, a + b);
}

TEST(RatOperatorTests, UnMinusZero) {
    Rational a = 0;
    Rational b = -a;
    ASSERT_EQ(a, b);
}   

TEST(RatOperatorTests, UnMinusNeg) {
    Rational a = -1;
    Rational b = -a;
    ASSERT_EQ(1, b);
}

TEST(RatMethodTests, ToString) {
    Rational a = 11;
    a /= 7;
    ASSERT_EQ("11/7", a.toString());
}

TEST(RatMethodTests, ToStringNeg) {
    Rational a = -179;
    a /= 57;
    ASSERT_EQ("-179/57", a.toString());
}

TEST(RatMethodTests, ToStringNegDenom) {
    Rational a = 179;
    a /= -57;
    ASSERT_EQ("-179/57", a.toString());
}

TEST(RatMethodTests, ToStringZero) {
    Rational a = 0;
    ASSERT_EQ("0", a.toString());
}

TEST(RatMethodTests, ToStringNatural) {
    Rational a = 179;
    ASSERT_EQ("179", a.toString());
}

TEST(RatMethodTests, ToStringReduction) {
    Rational a = 12;
    a /= 20;
    ASSERT_EQ("3/5", a.toString());
}

TEST(RatMethodTests, AsDecimal) {
    Rational a = 179;
    ASSERT_EQ("179.0", a.asDecimal(1));
}

TEST(RatMethodTests, AsDecimalZeroPrec) {
    Rational a = 11;
    ASSERT_EQ("11", a.asDecimal(0));
}

TEST(RatMethodTests, AsDecimalRound) {
    Rational a = 11;
    a /= 20;
    ASSERT_EQ("0.6", a.asDecimal());
}

TEST(RatMethodTests, AsDecimalMoreZeroes) {
    Rational a = 11;
    a /= 20;
    ASSERT_EQ("0.55000", a.asDecimal(5));
}

TEST(RatMethodTests, AsDecimalRoundDown) {
    Rational a = 10;
    a /= 3;
    ASSERT_EQ("3.3", a.asDecimal(1));
} 

TEST(RatMethodTests, AsDecimalRoundUp) {
    Rational a = 5;
    a /= 3;
    ASSERT_EQ("1.7", a.asDecimal(1));
}

TEST(RatOperatorTests, DoubleCast) {
    Rational r = 1;
    double d = 1;
    d /= 3;
    r /= 3;
    double d1 = static_cast<double>(r);
    ASSERT_LE(precision, abs(d - d1));
}

TEST(RatOperatorTests, DoubleCastRandom) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        BigInteger numerator = random_bigint(18);
        double d = static_cast<long long>(numerator);
        Rational r = numerator;
        BigInteger denom = 0;
        while (denom == 0) denom = random_bigint(18);
        d /= static_cast<long long>(denom);
        r /= denom;
        double d1 = static_cast<double>(r);

        ASSERT_LE(precision, abs(d-d1));
    }
}

TEST(RatOperatorTests, BoolCastTrue) {
    Rational a = 179;
    a /= 57;
    ASSERT_TRUE(static_cast<bool>(a));
}

TEST(RatOperatorTests, BoolCastFalse) {
    Rational a = 0;
    ASSERT_FALSE(static_cast<bool>(a));
}

TEST(RatOperatorTests, BoolCastNeg) {
    Rational a = -1;
    a /= 179;
    ASSERT_TRUE(static_cast<bool>(a));
}

TEST(RatOperatorTests, SpaceshipLess) {
    Rational a = 1;
    Rational b = 2;
    a /= 3;
    b /= 5;
    ASSERT_EQ(std::strong_ordering::less, a <=> b);
}

TEST(RatOperatorTests, SpaceshipEquiv) {
    Rational a = 1;
    a /= 3;
    Rational b = 3;
    b /= 9;
    ASSERT_EQ(std::strong_ordering::equivalent, a <=> b);
}

TEST(RatOperatorTests, SpaceshipGreater) {
    Rational a = 1;
    a /= 3;
    Rational b = 2;
    b /= 7;
    ASSERT_EQ(std::strong_ordering::greater, a <=> b);
}

TEST(RatOperatorTests, SpaceshipNeg) {
    Rational a = -1;
    a /= 3;
    Rational b = 1;
    b /= 8;
    ASSERT_EQ(std::strong_ordering::less, a <=> b);
}

TEST(RatOperatorTests, SpaceshipNeg2) {
    Rational a = -1;
    a /= 8;
    Rational b = 1;
    b /= 3;
    ASSERT_EQ(std::strong_ordering::less, a <=> b);
}

TEST(RatOperatorTests, SpaceshipNegNeg) {
    Rational a = -1;
    a /= 3;
    Rational b = -1;
    b /= 5;
    ASSERT_EQ(std::strong_ordering::less, a <=> b);
}

TEST(RatOperatorTests, SpacechipZeroZero) {
    Rational a = 0;
    a /= 179;
    Rational b = 0;
    b /= 57;

    ASSERT_EQ(std::strong_ordering::equivalent, a <=> b);
}

TEST(RatOperatorTests, SpaceshipTime) {
    int total_time = 0;
    int time_treshold = 1000;

    for (int i = 0; i < 100; ++i) {
        Rational a = random_rational(10000);
        Rational b = random_rational(10000);
        Timer T;
        T.start();
        a <=> b;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(RatOperatorTests, Less) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        Rational a = random_rational(10000);
        Rational b = random_rational(10000);
        auto result = a <=> b;
        bool answer = result == std::strong_ordering::less;
        ASSERT_EQ(answer, a < b);
    }
}

TEST(RatOperatorTests, LEq) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        Rational a = random_rational(10000);
        Rational b = random_rational(10000);
        auto result = a <=> b;
        bool answer = result != std::strong_ordering::greater;
        ASSERT_EQ(answer, a <= b);
    }
}

TEST(RatOperatorTests, Greater) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        Rational a = random_rational(10000);
        Rational b = random_rational(10000);
        auto result = a <=> b;
        bool answer = result == std::strong_ordering::greater;
        ASSERT_EQ(answer, a > b);
    }
}

TEST(RatOperatorTests, GEq) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        Rational a = random_rational(10000);
        Rational b = random_rational(10000);
        auto result = a <=> b;
        bool answer = result != std::strong_ordering::less;
        ASSERT_EQ(answer, a >= b);
    }
}

TEST(RatOperatorTests, EqTrue) {
    Rational a = 1;
    a /= 3;
    Rational b = 1;
    b /= 3;
    ASSERT_TRUE(a == b);
}

TEST(RatOperatorTests, EqReduction) {
    Rational a = 1;
    a /= 3;
    Rational b = 2;
    b /= 6;
    ASSERT_TRUE(a == b);
}

TEST(RatOperatorTests, EqFalse) {
    Rational a = 1;
    a /= 3;
    Rational b = 1;
    b /= 5;
    ASSERT_FALSE(a == b);
} 

TEST(RatOperatorTests, EqNegTrue) {
    Rational a = -1;
    a /= 3;
    Rational b = -1;
    b /= 3;
    ASSERT_TRUE(a == b);
}

TEST(RatOperatorTests, EqNegPos) {
    Rational a = -1;
    a /= 3;
    Rational b = 1;
    b /= 3;
    ASSERT_FALSE(a == b);
}

TEST(RatOperatorTests, EqTime) {
    int total_time = 0;
    int time_treshold = 500;
    for (int i = 0; i < 100; ++i) {
        Rational a = random_rational(100'000);
        Rational b = random_rational(100'000);
        Timer T;
        T.start();
        a == b;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(RatOperatorTests, NeqRandom) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        Rational a = random_rational(10000);
        Rational b = random_rational(10000);

        ASSERT_EQ(!(a == b), a != b);
    }
}

#define CHECK_SAME_OPERATOR_RAT(op, correctOp) \
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) { \
        Rational a = random_rational(10000); \
        Rational b = random_rational(10000); \
        Rational c = a op b; \
        ASSERT_EQ(a correctOp b, c); \
    }

TEST(RatOperatorTests, PlusRandom) {
    CHECK_SAME_OPERATOR_RAT(+, +=);
}

TEST(RatOperatorTests, MinusRandom) {
    CHECK_SAME_OPERATOR_RAT(-, -=);
}

TEST(RatOperatorTests, MultRandom) {
    CHECK_SAME_OPERATOR_RAT(*, *=);
}

TEST(RatOperatorTests, DivRandom) {
    CHECK_SAME_OPERATOR_RAT(/, /=);
}

