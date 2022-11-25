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

Rational::Rational(const BigInteger& value) : Rational(value, 1) {}

Rational::Rational(const BigInteger& numerator, const BigInteger& denominator) : numerator(numerator), denominator(denominator) {
    reduct();
}

Rational& Rational::operator+=(const Rational& other) {
    numerator = numerator * other.denominator + denominator * other.numerator;
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

Rational Rational::operator-() const {
    return Rational(-numerator, denominator);
}

string Rational::toString() const {
    string result = numerator.toString();
    if (denominator != 1) result += "/" + denominator.toString();
    return result;
}

string Rational::asDecimal(size_t precision) const {
    BigInteger additional = BigInteger::power(10, precision + 1);
    BigInteger pre_divided = ((numerator * additional) / denominator);
    string divided = ((pre_divided + 5) / 10).toString();
    string result = "";
    if (divided.size() <= precision) {
        result = "0.";
        for (size_t i = divided.size(); i < precision; ++i) {
            result += '0';
        }
    }
    for (size_t i = 0; i < divided.size(); ++i) { 
        result += divided[i];
        if (divided.size() - i - 1 == precision && precision > 0) {
            result += '.';
        }
    }
    return result;
}

BigInteger Rational::to_int_binary_shifted(long long binary_shift) const {
    BigInteger exponent = BigInteger::power(2, abs(binary_shift));
    BigInteger result = numerator;
    if (binary_shift < 0) {
        result /= exponent;
    } else {
        result *= exponent;
    }
    result /= denominator;
    return result;
}

Rational::operator double() const {
    if (numerator.is_zero()) return 0;
    long long exponent_l = -1024;
    long long exponent_r = 1024;
    while(exponent_l != exponent_r - 1) {
        long long m = (exponent_l + exponent_r) / 2;
        
        BigInteger result = to_int_binary_shifted(m);
        if (result.is_negative()) result.invert_sign();

        if (result >= DOUBLE_MAX) {
            exponent_l = m;
        } else {
            exponent_r = m;
        }
    }
    
    auto mantissa = to_int_binary_shifted(exponent_r);

    if (mantissa >= DOUBLE_MAX) throw TooBigCastException(mantissa, typeid(double));
    
    return ldexp(static_cast<long long>(mantissa), -exponent_r);
}

Rational::operator bool() const {
    return static_cast<bool>(numerator);
}

bool Rational::is_zero() const {
    return numerator.is_zero();
}

bool Rational::is_negative() const {
    return numerator.is_negative();
}

strong_ordering operator<=>(const Rational& left, const Rational& right) {
    auto left_bi = left.numerator * right.denominator;
    auto right_bi = right.numerator * left.denominator;
    return left_bi <=> right_bi;
}

bool operator==(const Rational& left, const Rational& right) {
    return left.numerator == right.numerator && left.denominator == right.denominator;
}

bool operator!=(const Rational& left, const Rational& right) {
    return !(left == right);
}

Rational operator+(const Rational& left, const Rational& right) {
    auto result = left;
    return result += right;
}

Rational operator-(const Rational& left, const Rational& right) {
    auto result = left;
    return result -= right;
}

Rational operator*(const Rational& left, const Rational& right) {
    auto result = left;
    return result *= right;
}

Rational operator/(const Rational& left, const Rational& right) {
    auto result = left;
    return result /= right;
}

std::ostream& operator<<(std::ostream& output, const Rational& other) {
    output << other.toString();
    return output;
}

