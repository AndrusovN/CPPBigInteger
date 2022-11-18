#pragma once

#include <gtest/gtest.h>
#include "biginteger.h"
#include "helper.h"

using std::string;

TEST(ExceptionTests, CStrFromStr) {
    string message = "this is message";
    char* res = rebuild_c_string_from_string(message);
    ASSERT_EQ(message, res);
    delete[] res;
}

TEST(ExceptionTests, InvalidInput) {
    try {
        throw InvalidInputException("aboba");
    } catch (std::exception& e) {
        ASSERT_EQ("Invalid input to create BigInteger: aboba. Expected a numerical literal", string(e.what()));
    }
}

TEST(ExceptionTests, DivisionByZero) {
    try {
        throw DivisionByZero(BigInteger(179));
    } catch (std::exception& e) {
        ASSERT_EQ("Division by zero! Trying to divide 179 by zero", string(e.what()))
    }
}

TEST(ExceptionTests, Cast) {
    try {
        throw CastException(BigInteger(1791791791), typeid(char));
    } catch (std::exception& e) {
        ASSERT_EQ("Exception during cast. Value to cast: 1791791791 type to cast: c", string(e.what()));
    }
}

TEST(ExceptionTests, TooBigCast) {
    try {
        throw TooBigCastException(BigInteger(1791791791), typeid(char));
    } catch (std::exception& e) {
        ASSERT_EQ("Too big cast. Exception during cast. Value to cast: 1791791791 type to cast: c", string(e.what()));
    }
}

TEST(ExceptionTests, NegativeToUnsignedCast) {
    try {
        throw NegativeToUnsignedCastException(BigInteger(1791791791), typeid(char));
    } catch (std::exception& e) {
        ASSERT_EQ("Trying to cast negative value to unsigned type. Exception during cast. Value to cast: 1791791791 type to cast: c", string(e.what()));
    }
} 

TEST(BiConstructorTests, StringException) {
    ASSERT_THROW(BigInteger a = "179qwerty", InvalidInputException);
}

TEST(BiOperatorTests, LLCastTooBig) {
    BigInteger a = "179179179179179179179179179179";
    ASSERT_THROW(static_cast<long long>(a), TooBigCastException);
}

TEST(BiOperatorTests, ULLCastNegError) {
    BigInteger a = -179;

    ASSERT_THROW(static_cast<unsigned long long>(a), NegativeToUnsignedCastException);
}

TEST(BiOperatorTests, ULLCastTooBig) {
    BigInteger a = "179179179179179179179179179179";

    ASSERT_THROW(static_cast<unsigned long long>(a), TooBigCastException);
}

TEST(BiOperatorTests, InputError) {
    std::stringstream testStream;
    testStream << "179acb";
    BigInteger a;
    ASSERT_THROW(testStream >> a, InvalidInputException);
}

TEST(BiOperatorTests, ModZeroDivisionExc) {
    BigInteger a = 179;
    ASSERT_THROW(a % 0, DivisionByZeroException);
}

TEST(BiOperatorTests, DivZeroDivisionExc) {
    BigInteger a = 179;
    ASSERT_THROW(a / 0, DivisionByZeroException);
}


