#include "rational.h"

void Rational::reduct() {
    BigInteger to_reduct = gcd(numerator, denominator);
    numerator /= to_reduct;
    denominator /= to_reduct;

    if (denominator.is_negative()) {
        numerator.invert_sign();
        denominator.invert_sign();
    }
}

Rational::Rational() : numerator(0), denominator(1) {}

Rational::Rational(long long value) : Rational(BigInteger(value)) {}

Rational::Rational(const BigInteger& value) : numerator(value), denominator(1) {}

Rational& Rational::operator+=(const Rational& other) {
    numerator *= other.denominator;
    numerator += other.numerator * denominator;
    denominator *= other.denominator;
    reduct();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    return *this += -other;
}

Rational& Rational::operator*=(const Rational& other) {
    numerator *= other.numerator;
    denominator *= other.denominator;
    reduct();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    if (other.numerator.is_zero()) throw DivisionByZeroException(numerator);
    numerator *= other.denominator;
    denominator *= other.numerator;
    reduct();
    return *this;
}

