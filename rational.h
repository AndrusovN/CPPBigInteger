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
  public:

    Rational();

    Rational(long long value);

    Rational(const BigInteger& source);

    Rational& operator+=(const Rational& other);

    Rational& operator-=(const Rational& other);

    Rational& operator*=(const Rational& other);

    Rational& operator/=(const Rational& other);

    Rational operator-();

    string toString() const;

    string asDecimal(size_t precision=0) const;

    explicit operator double() const;

    explicit operator bool() const;
};

auto operator<=>(const Rational& left, const Rational& right);

bool operator==(const BigInteger& left, const BigInteger& right);

bool operator!=(const BigInteger& left, const BigInteger& right);

Rational operator+(const Rational& left, const Rational& right);

Rational operator-(const Rational& left, const Rational& right);

Rational operator*(const Rational& left, const Rational& right);

Rational operator/(const Rational& left, const Rational& right);

