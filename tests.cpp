#include <compare>
#include <sstream>
#include <gtest/gtest.h>
#include <random>

#include "biginteger.h"
#include "rational.h"
#include "helper.h"

#include "bigint_test_helper.h"
#include "exceptions_tests.h"
#include "bigint_arithmetics_tests.h"
#include "bigint_types_tests.h"
#include "bigint_equalities_tests.h"
#include "rational_tests.h"


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

