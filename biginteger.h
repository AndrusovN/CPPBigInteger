#pragma once

#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

class InvalidInputException: public std::exception {
  private:
    string input;

  public:
    InvalidInputException(string input) : input(input) {}
   
   const char* what() const noexcept override {
        auto data = "Invalid input to create BigInteger: " +
            input + ". Expected a numerical literal";
        char* result = new char[data.size() + 1]();
        std::copy(data.c_str(), data.c_str() + data.size(), result);
        return result;
    }
};

class DivisionByZeroException: public std::exception {
  private:
    BigInteger value;
  public:
    DivisionByZeroException(BigInteger value) : value(value) {}

    const char* what() const noexcept override {
        auto data = "Division by zero! Trying to divide " + value.toString() + " by zero";
        char* result = new char[data.size() + 1]();
        std::copy(data.c_str(), data.c_str() + data.size(), result);
        return result;
    } 
}


class BigInteger {
  private:
    

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

    BigInteger operator-();

    string toString() const;
    
    operator long long() const;

    operator unsigned long long() const;

    explicit operator bool() const;

    ~BigInteger();
};

bool operator==(const BigInteger& left, const BigInteger& right);

bool operator!=(const BigInteger& left, const BigInteger& right);

BigInteger operator+(const BigInteger& left, const BigInteger& right);

BigInteger operator-(const BigInteger& left, const BigInteger& right);

BigInteger operator*(const BigInteger& left, const BigInteger& right);

BigInteger operator/(const BigInteger& left, const BigInteger& right);

BigInteger operator%(const BigInteger& left, const BigInteger& right);

BigInteger operator-(const BigInteger& source);

std::istream& operator>>(std::istream& input, BigInteger& value);

std::ostream& operator<<(std::ostream& output, const BigInteger& source);

auto operator<=>(const BigInteger& left, const BigInteger& right);

BigInteger& operator""_bi(unsigned long long);

class Rational {
  private:


  public:

    Rational();

    Rational(long long value);

    Rational(const BigInteger& source);

    Rational& operator+=(const Rational& other);

    Rational& operator-=(const Rational& other);

    Rational& operator*=(const Rational& other);

    Rational& operator/=(const Rational& other);

    string toString() const;

    string asDecimal(size_t precision=0) const;

    operator double() const;

    explicit operator bool() const;
};

auto operator<=>(const Rational& left, const Rational& right);

bool operator==(const BigInteger& left, const BigInteger& right);

bool operator!=(const BigInteger& left, const BigInteger& right);

Rational& operator-(const Rational& source);

Rational operator+(const Rational& left, const Rational& right);

Rational operator-(const Rational& left, const Rational& right);

Rational operator*(const Rational& left, const Rational& right);

Rational operator/(const Rational& left, const Rational& right);

