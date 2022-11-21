#pragma once

#include <compare>
#include <complex>
#include <vector>
#include <string>
#include <iostream>

#include "exceptions.h"

using std::vector;
using std::string;
using std::strong_ordering;

using digit_t = unsigned char;
using complex = std::complex<long double>;

class BigInteger {
  private:
    static const digit_t BASE = 10;
    vector<digit_t> digits;
    bool negative = false;

    // REWRITE IF BASE CHANGES!!!!
    static digit_t char_to_digit(char c);

    static char digit_to_char(digit_t d);

    void add_absolute(const BigInteger& other);

    void substract_absolute(const BigInteger& other);

    strong_ordering compare_absolute(const BigInteger& other) const;

    // contract: reduced is bigger than substracted
    static void substract_vectors(const vector<digit_t>& reduced, const vector<digit_t>& substracted, vector<digit_t>& difference);
  
    // Applies Fast Fourier Transform (or it's inversed form) to the vector of coefficients (or values respectively)
    static void fft(vector<complex>& coefficients, bool inversed=false);
    
    static vector<digit_t> complex_to_digits(const vector<complex>& values);

    static vector<complex> digits_to_complex(const vector<digit_t>& values, size_t target_size);

    BigInteger shift(int digits) const;
  public:
    BigInteger();

    BigInteger(long long value);

    BigInteger(unsigned long long value);

    BigInteger(const BigInteger& source);

    BigInteger(const string& source);

    BigInteger& operator=(const BigInteger& source);

    BigInteger& operator+=(const BigInteger& other);

    BigInteger& operator-=(const BigInteger& other);

    BigInteger& operator*=(const BigInteger& other);

    BigInteger& operator/=(const BigInteger& other);

    BigInteger& operator%=(const BigInteger& other);

    BigInteger& operator++();

    BigInteger operator++(int);

    BigInteger operator--();

    BigInteger operator--(int);

    BigInteger operator-() const;

    string toString() const;
    
    operator long long() const;

    operator unsigned long long() const;

    explicit operator bool() const;

    size_t size() const;
    
    bool is_zero() const;   
    
    bool is_negative() const;

    void invert_sign();

    ~BigInteger();

    friend strong_ordering operator<=>(const BigInteger& left, const BigIntegr& right);
};

bool operator==(const BigInteger& left, const BigInteger& right);

bool operator!=(const BigInteger& left, const BigInteger& right);

BigInteger operator+(const BigInteger& left, const BigInteger& right);

BigInteger operator-(const BigInteger& left, const BigInteger& right);

BigInteger operator*(const BigInteger& left, const BigInteger& right);

BigInteger operator/(const BigInteger& left, const BigInteger& right);

BigInteger operator%(const BigInteger& left, const BigInteger& right);

std::istream& operator>>(std::istream& input, BigInteger& value);

std::ostream& operator<<(std::ostream& output, const BigInteger& source);

BigInteger operator""_bi(unsigned long long);

BigInteger gcd(const BigInteger& left, const BigInteger& right);

