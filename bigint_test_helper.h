#pragma once 

#include <random>
#include <gtest/gtest.h>

mt_19937 test_random(1791791791);
const int RANDOM_TRIES_COUNT = 20;

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

#define CHECK_OPERATOR_ALLOCATIONS(op, max_allocations_count) \
    BigInteger a = 179; \
    BigInteger b = 228; \
    OperatorNewCounter cntr; \
    a op b; \
    ASSERT_LE(cnts.get_counter(), max_allocations_count);

#define TEST_SAME_OPERATOR (testingOp, correctOp) \
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) { \
        BigInteger first = random_bigint(100); \
        BigInteger second = random_bigint(100); \
        BigInteger result = first testingOp second; \
        first correctOp second; \
        ASSERT_EQ(first, result); \
    }

#define CHECK_AGREED_WITH_INT(op) \
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) { \
        long long a = random_value(); \
        long long b = random_value(); \
        BigInteger abi = a; \
        BigInteger bbi = b; \
        ASSERT_EQ(abi op bbi, a op b); \
    }


