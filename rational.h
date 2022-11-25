#pragma once

#include <compare>
#include <string>
#include <iostream>

#include "exceptions.h"

using std::string;
using std::strong_ordering;

class Rational {
  private:
    BigInteger numerator;
    BigInteger denominator;

    void reduct();

    BigInteger to_int_binary_shifted(long long binary_shift) const;
  public:

    Rational();

    Rational(long long value);

    Rational(const BigInteger& value);

    Rational(const BigInteger& numerator, const BigInteger& denominator);

    Rational& operator+=(const Rational& other);

    Rational& operator-=(const Rational& other);

    Rational& operator*=(const Rational& other);

    Rational& operator/=(const Rational& other);

    Rational operator-() const;

    string toString() const;

    string asDecimal(size_t precision=0) const;

    explicit operator double() const;

    explicit operator bool() const;

    bool is_zero() const;

    bool is_negative() const;
    
    friend strong_ordering operator<=>(const Rational& left, const Rational& right);

    friend bool operator==(const Rational& left, const Rational& right);
};

bool operator!=(const BigInteger& left, const BigInteger& right);

Rational operator+(const Rational& left, const Rational& right);

Rational operator-(const Rational& left, const Rational& right);

Rational operator*(const Rational& left, const Rational& right);

Rational operator/(const Rational& left, const Rational& right);

const BigInteger DOUBLE_MAX = BigInteger(1ll << 52);

std::ostream& operator<<(std::ostream& output, const Rational& other);

