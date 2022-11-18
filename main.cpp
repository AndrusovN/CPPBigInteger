#include <compare>
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

TEST(BiConstructorTests, ULL) {
    unsigned long long value = 888888888888888888ull;
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
    BigInteger a = std::to_string(num);
    
    ASSERT_EQ(num, a);
}

TEST(BiConstructorTests, StringZeroes) {
    BigInteger a = "00000";

    ASSERT_EQ(0, a);
}

TEST(BiConstructorTests, StringNegative) {
    BigInteger a = "-179";

    ASSERT_EQ(-179, a);
}

TEST(BiConstructorTests, StringZeroesBegin) {
    BigInteger a = "000179";

    ASSERT_EQ(179, a);
}

TEST(BiConstructorTests, StringException) {
    ASSERT_ANY_THROW(BigInteger a = "179qwerty");
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

#define CHECK_OPERATOR_ALLOCATIONS(op, max_allocations_count) \
    BigInteger a = 179; \
    BigInteger b = 228; \
    OperatorNewCounter cntr; \
    a op b; \
    ASSERT_LE(cnts.get_counter(), max_allocations_count);


TEST(BiOperatorTests, PlusEQ) {
    BigInteger a = 179;
    a += 179;
    
    ASSERT_EQ(179l+179l, a);
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
    for (auto i = 0; i < 20; ++i) {
        long long first = random_value();
        long long second = random_value();
        BigInteger a = first;
        a += second;
        ASSERT_EQ(first + second, a);
    }
}

TEST(BiOperatorTests, PlusEQTime) {
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
    for (auto i = 0; i < 20; ++i) {
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
    for (auto i = 0; i < 20; ++i) {
        test_mult(random_value(), random_value());
    }
}

TEST(BiOperatorTests, TimesEQTime) {
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

TEST(BiOperatorTests, TimesEqMemory) {
    CHECK_OPERATOR_ALLOCATES(*=, 2);
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
    test_division(-179, 57)
}

TEST(BiOperatorTests, DivNegNeg) {
    test_division(-179, -57);
}

TEST(BiOperatorTests, DivRandom) {
    for (int i = 0; i < 20; ++i) {
        auto left = random_value();
        auto right = 0;
        while (right == 0) right = random_value();
        test_division(left, right);
    }
}

TEST(BiOperatorTests, DivZeroDivisionExc) {
    BigInteger a = 179;
    ASSERT_THROW(a / 0, DivisionByZeroException);
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
        T.end();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(BiOperatorTests, DivMemory) {
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

TEST(BiOperatorTests, ModZeroDivisionExc) {
    BigInteger a = 179;
    ASSERT_ANY_THROW(a % 0);
}

TEST(BiOperatorTests, ModRandom) {
    for (int i = 0; i < 20; ++i) {
        long long left = random_value();
        long long right = 0;
        while (right == 0) right = random_value();

        test_modulus(left, right);
    }
}

TEST(BiOperatorTests, ModTime) {
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

TEST(BiOperatorTests, ModMemory) {
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
    b = a++;
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

TEST(BiMethodTests, ToString) {
    BigInteger a = 1791791791;

    ASSERT_EQ("1791791791", a.toString());
}

TEST(BiMethodTests, ToStringFromString) {
    string s = "476321836354832172498732156235748521692374863219823435621786345367821";
    BigInteger a = s;

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
    BigInteger a = "000000000179";

    ASSERT_EQ("179", a.toString());
}

TEST(BiOperatorTests, LLCast) {
    BigInteger a = 1791791791;
    long long b = a;

    ASSERT_EQ(a, b);
}

TEST(BiOperatorTests, LLCastNeg) {
    BigInteger a = -179179179179179179l;
    long long b = a;

    ASSERT_EQ(a, b);
}

TEST(BiOperatorTests, LLCastTooBig) {
    BigInteger a = "179179179179179179179179179179";
    ASSERT_THROW(static_cast<long long>(a), TooBigCastException);
}

TEST(BiOperatorTests, LLCastMemory) {
    BigInteger a = 11;
    OperatorNewCounter cntr;
    auto b = static_cast<long long>(a);
    ASSERT_EQ(0, cntr.get_counter());
}

TEST(BiOperatorTests, ULLCast) {
    BigInteger a = 1791791791;
    unsigned long long b = a;
    
    ASSERT_EQ(a, b);
}

TEST(BiOperatorTests, ULLCastZero) {
    BigInteger a = 0;
    unsigned long long b = a;

    ASSERT_EQ(a, b);
}

TEST(BiOperatorTests, ULLCastNegError) {
    BigInteger a = -179;

    ASSERT_THROW(static_cast<unsigned long long>(a), NegativeToUnsignedCastException);
}

TEST(BiOperatorTests, ULLCastBigOK) {
    BigInteger a = 1e19;
    unsigned long long b;

    ASSERT_NO_THROW(b = a);
    ASSERT_EQ(a, b);
}

TEST(BiOperatorTests, ULLCastTooBig) {
    BigInteger a = "179179179179179179179179179179";

    ASSERT_THROW(static_cast<unsigned long long>(a), TooBigCastException);
}

TEST(BiOperatorTests, ULLCastMemory) {
    BigInteger a = 1791791791;
    OperatorNewCounter cntr;
    static_cast<unsigned long long>(a);
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

#define TEST_SAME_OPERATOR (testingOp, correctOp) \
    for (int i = 0; i < 20; ++i) { \
        BigInteger first = random_bigint(100); \
        BigInteger second = random_bigint(100); \
        BigInteger result = first testingOp second; \
        first correctOp second; \
        ASSERT_EQ(first, result); \
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

#define CHECK_AGREED_WITH_INT(op) \
    for (int i = 0; i < 20; ++i) { \
        long long a = random_value(); \
        long long b = random_value(); \
        BigInteger abi = a; \
        BigInteger bbi = b; \
        ASSERT_EQ(abi op bbi, a op b); \
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

TEST(BiOperatorTests, InputError) {
    std::stringstream testStream;
    testStream << "179acb";
    BigInteger a;
    ASSERT_THROW(testStream >> a, InvalidInputException);
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
    for (int i = 0; i < 20; ++i) {
        BigInteger a = random_bigint(1000);
        ss << a;
        BigInteger b;
        ss >> b;
        ASSERT_EQ(a, b);
    }
}

TEST(BiOperatorTests, Literal) {
    auto a = "1791791791"_bi;
    ASSERT_EQ(1791791791, a);
}

TEST(BiOperatorTests, LiteralNeg) {
    auto a = "-1791791791"_bi;
    ASSERT_EQ(-1791791791, a);
}

TEST(BiOperatorTests, LiteralZero) {
    auto a = "000000000"_bi;
    ASSERT_EQ(0, a);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

