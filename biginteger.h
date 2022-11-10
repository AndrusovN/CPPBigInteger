#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

class BaseException {
  public:
    virtual void printError() const = 0;

    ~BaseException() {}
};

class InvalidInputException: public Exception {
  private:
    string input;

  public:
    InvalidInputException(string input) : input(input) {}

    void printError() const override {
        std::cerr << "Invalid input to create BigInteger: " << 
            input << " expected a numerical literal" << std::endl;
    }
};


class BigInteger {
  private:
    

  public:
    BigInteger();

    BigInteger(long long value);

    BigInteger(unsigned long long value);

    BigInteger(const BigInteger& source);

    BigInteger(const string& source);

    BigInteger& operator = (const BigInteger& source);

    BigInteger& operator += (const BigInteger& other);

    BigInteger& operator -= (const BigInteger& other);

    BigInteger& operator *= (const BigInteger& other);

    BigInteger& operator /= (const BigInteger& other);

    BigInteger& operator %= (const BigInteger& other);

    BigInteger& operator ++ ();

    BigInteger operator ++ (int);

    BigInteger operator -- ();

    BigInteger operator -- (int);

    string toString() const;
    
    operator long long() () const;

    operator unsigned long long() () const;

    ~BigInteger();
};

BigInteger operator + (BigInteger left, const BigInteger& right);

BigInteger operator - (BigInteger left, const BigInteger& right);

BigInteger operator * (BigInteger left, const BigInteger& right);

BigInteger operator / (BigInteger left, const BigInteger& right);

BigInteger operator % (BigInteger left, const BigInteger& right);

BigInteger operator - (BigInteger source);

std::istream& operator >> (std::istream& input, BigInteger& value);

std::ostream& operator << (std::ostream& output, const BigInteger& source);

auto operator <=> (const BigInteger& left, const BigInteger& right);

BigInteger& operator "" _bi(long long);

class Rational {
  private:


  public:

    Rational();

    Rational(long long value);

    Rational(const BigInteger& source);

    Rational& operator += (const Rational& other);

    Rational& operator -= (const Rational& other);

    Rational& operator *= (const Rational& other);

    Rational& operator /= (const Rational& other);

    string toString() const;

    string asDecimal(size_t precision=0) const;

    operator double() () const;
};

auto operator <=> (const Rational& left, const Rational& right);

Rational& operator - (Rational source);

Rational operator + (Rational left, const Rational& right);

Rational operator - (Rational left, const Rational& right);

Rational operator * (Rational left, const Rational& right);

Rational operator / (Rational left, const Rational& right);

