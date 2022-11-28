#pragma once

#include <compare>
#include <complex>
#include <vector>
#include <string>
#include <iostream>
#include <assert.h>
#include <limits>
#include <math.h>

using std::vector;
using std::string;
using std::strong_ordering;

using digit_t = char;
using complex = std::complex<long double>;

size_t revert_binary(size_t index, size_t length);

class BigInteger {
  private:
    static const digit_t BASE = 10;
    vector<digit_t> digits;
    bool negative = false;

    // REWRITE IF BASE CHANGES!!!!
    static digit_t char_to_digit(char c);
    static char digit_to_char(digit_t d);

    // contract: reduced is bigger than substracted
    static void substract_vectors(const vector<digit_t>& reduced, const vector<digit_t>& substracted, vector<digit_t>& difference);
  
    // Applies Fast Fourier Transform (or it's inversed form) to the vector of coefficients (or values respectively)
    static void fft(vector<complex>& coefficients, bool inversed=false);
    
    static vector<digit_t> complex_to_digits(const vector<complex>& values);

    static vector<complex> digits_to_complex(const vector<digit_t>& values, size_t target_size); 

    static void clear_leading_zeroes(vector<digit_t>& digits);

    static void reorder_for_fft(vector<complex>& source);


    void add_absolute(const BigInteger& other);

    void substract_absolute(const BigInteger& other);

    strong_ordering compare_absolute(const BigInteger& other) const;

    void resolve_carry(int carry, size_t index);

    void resolve_sign();

  public:
    BigInteger();

    BigInteger(long long value);

    BigInteger(const BigInteger& source);

    explicit BigInteger(const string& source);

    BigInteger& operator=(const BigInteger& source);

    BigInteger& operator+=(const BigInteger& other);

    BigInteger& operator-=(const BigInteger& other);

    BigInteger& operator*=(const BigInteger& other);

    BigInteger& operator/=(const BigInteger& other);

    BigInteger& operator%=(const BigInteger& other);

    BigInteger& operator++();

    BigInteger operator++(int);

    BigInteger& operator--();

    BigInteger operator--(int);

    BigInteger operator-() const;

    string toString() const;
    
    explicit operator long long() const;

    explicit operator bool() const;

    size_t size() const;
    
    bool is_zero() const;   
    
    bool is_negative() const;

    void invert_sign();

    void shift(int digits);

    ~BigInteger() = default;

    static BigInteger power(const BigInteger& indicator, const BigInteger& exponent);

    friend strong_ordering operator<=>(const BigInteger& left, const BigInteger& right);
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

BigInteger gcd(BigInteger left, BigInteger right);

BigInteger longMin = BigInteger(std::numeric_limits<long long>::min());
BigInteger longMax = BigInteger(std::numeric_limits<long long>::max()); 

size_t revert_binary(size_t index, size_t length) {
    size_t result = 0;
    for (size_t i = 0; i < length; ++i) {
        if ((index >> i) & 1) {
            result |= (1 << (length - i - 1));
        }
    }
    return result;
}

size_t BigInteger::size() const {
    return digits.size();
}

void BigInteger::resolve_sign() {
    if (is_zero()) negative = false;
}

digit_t BigInteger::char_to_digit(char c) {
    return c - '0';
}

char BigInteger::digit_to_char(digit_t d) {
    return '0' + d;
}

bool BigInteger::is_zero() const {
    return size() == 1 && digits[0] == 0;
}

void BigInteger::clear_leading_zeroes(vector<digit_t>& digits) {
    while(digits.size() > 1 && digits.back() == 0) digits.pop_back();
}

strong_ordering BigInteger::compare_absolute(const BigInteger& other) const {
    if (size() > other.size()) return strong_ordering::greater;
    if (size() < other.size()) return strong_ordering::less;
    for (size_t i = size(); i > 0; --i) {
        size_t index = i - 1;
        if (digits[index] > other.digits[index]) return strong_ordering::greater;
        if (digits[index] < other.digits[index]) return strong_ordering::less;
    }
    return strong_ordering::equivalent;
}

void BigInteger::resolve_carry(int carry, size_t index) {
    while(carry != 0) {
        if (index == size()) digits.push_back(0);
        carry += digits[index];
        digits[index] = carry % BASE;
        if (carry < 0) {
            digits[index] += BASE;
            carry /= BASE;
            --carry;
        } else {
            carry /= BASE;
        }
        ++index;
    }
}

void BigInteger::add_absolute(const BigInteger& other) {
    digit_t carry = 0;
    digits.reserve(other.size() + 1);

    for (size_t i = 0; i < other.size(); ++i) {
        if (i == size()) digits.push_back(0);
        digits[i] += other.digits[i];
        digits[i] += carry;
        carry = digits[i] / BASE;
        digits[i] %= BASE;
    }
    resolve_carry(carry, other.size());
}

void BigInteger::substract_vectors(const vector<digit_t>& reduced, const vector<digit_t>& substracted, vector<digit_t>& difference) {
    assert(substracted.size() <= reduced.size());
    
    difference.reserve(reduced.size());

    digit_t carry = 0;
    for (size_t i = 0; i < reduced.size(); ++i) {
        if (i == difference.size()) difference.push_back(0);
                
        digit_t to_substract = (i < substracted.size()) ? substracted[i] : 0;
        
        difference[i] = reduced[i] - carry - to_substract;
        carry = 0;
        if (difference[i] < 0) {
            difference[i] += BASE;
            carry = 1;
        }

        if (i >= substracted.size() && carry == 0) {
            if (&difference == &reduced) break;
        }
    }

    clear_leading_zeroes(difference);
    assert(carry == 0);
}

void BigInteger::substract_absolute(const BigInteger& other) {
    auto comparison_result = compare_absolute(other);
    
    if (comparison_result == strong_ordering::less) {
        substract_vectors(other.digits, digits, digits);
        negative = !negative;
    } else if(comparison_result == strong_ordering::equivalent) {
        digits.clear();
        digits.push_back(0);
        negative = false;
    } else {
        substract_vectors(digits, other.digits, digits);
    }
}

BigInteger::BigInteger() : BigInteger(0) {}

BigInteger::BigInteger(long long value) {
    bool special_case = false;
    if (value < 0) {
        negative = true;
        if (value == std::numeric_limits<long long>::min()) {
            special_case = true;
            ++value;
        }
        value = -value;
    }
    do {
        digits.push_back(value % BASE);
        value /= BASE;
    } while(value > 0);
    if (special_case) --*this;
}

BigInteger::BigInteger(const BigInteger& source) 
        : digits(source.digits.begin(), source.digits.end())
        , negative(source.negative) {}

BigInteger::BigInteger(const string& source) {
    size_t offset = 0;
    if (source[0] == '-') {
        negative = true;
        ++offset;
    }
    digits.resize(source.size() - offset);
    for (size_t i = source.size(); i > offset; --i) {
        digits[source.size() - i] = char_to_digit(source[i - 1]);
    }
    clear_leading_zeroes(digits);
}

BigInteger& BigInteger::operator=(const BigInteger& source) {
    digits.resize(source.size());
    negative = source.negative;
    for (size_t i = 0; i < source.size(); ++i) {
        digits[i] = source.digits[i];
    }
    return *this;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    if (negative == other.negative) {
        add_absolute(other);
    } else {
        substract_absolute(other);
    }
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    if (negative == other.negative) {
        substract_absolute(other);
    } else {
        add_absolute(other);
    }
    return *this;
}

vector<complex> BigInteger::digits_to_complex(const vector<digit_t>& values, size_t target_size) {
    assert(target_size > values.size());
    size_t result_size = 1;
    while(target_size > 0) {
        result_size *= 2;
        target_size /= 2;
    }
    vector<complex> result(result_size, 0);
    for (size_t i = 0; i < values.size(); ++i) {
        result[i] = static_cast<complex>(values[i]);
    }

    return result;
}

vector<digit_t> BigInteger::complex_to_digits(const vector<complex>& values) {
    vector<digit_t> result(values.size(), 0);
    int carry = 0;
    for (size_t i = 0; i < result.size(); ++i) {
        carry += static_cast<int>(values[i].real() + 0.5);
        result[i] = carry % BASE;
        carry /= BASE;
    }

    // since values is built with enough space (on the initialization of fft),
    // it should always contain enough zero digits to contain all the carry
    assert(carry == 0);
    
    clear_leading_zeroes(result);

    return result;
}

void BigInteger::reorder_for_fft(vector<complex>& source) {
    size_t length = 0;
    while((1u << length) < source.size()) ++length;

    for (size_t i = 0; i < source.size(); ++i) {
        if (i < revert_binary(i, length)) {
            std::swap(source[i], source[revert_binary(i, length)]);
        }
    }
}

void BigInteger::fft(vector<complex>& source, bool inversed) {
    complex root_coefficient = complex(-1.0, inversed ? -0.0 : 0.0);

    reorder_for_fft(source);

    for (size_t block_length = 2; block_length <= source.size(); block_length *= 2) {
        long double angle = 2 * M_PI / block_length * (inversed ? -1 : 1);
        root_coefficient = complex(cosl(angle), sinl(angle));

        for (size_t block_id = 0; block_id < source.size() / block_length; ++block_id) {
            size_t left_part = block_id * block_length;
            size_t right_part = left_part + block_length / 2;
           
            complex current_root = 1;

            for (size_t i = 0; i < block_length / 2; ++i) {
                complex first = source[left_part + i] + current_root * source[right_part + i];
                complex second = source[left_part + i] - current_root * source[right_part + i];
                source[left_part + i] = first;
                source[right_part + i] = second;
                current_root *= root_coefficient;
            } 
        }
        //root_coefficient = std::sqrt(root_coefficient);
    }

    if (inversed) {
        for (size_t i = 0; i < source.size(); ++i) {
            source[i] /= source.size();
        }
    }
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    auto my_complex_values = digits_to_complex(digits, size() + other.size());
    fft(my_complex_values);
    
    auto other_complex_values = digits_to_complex(other.digits, size() + other.size());
    fft(other_complex_values);

    for (size_t i = 0; i < my_complex_values.size(); ++i) {
        my_complex_values[i] *= other_complex_values[i];
    }

    fft(my_complex_values, true);
    digits = std::move(complex_to_digits(my_complex_values));
    negative ^= other.negative;
    if (is_zero() || other.is_zero()) negative = false;

    return *this;
}

void BigInteger::shift(int digits) {
    string characters = toString();
    for (int i = 0; i < digits; ++i) {
        characters.push_back('0');
    }
    for (int i = digits; i < 0 && characters.size(); ++i) {
        characters.pop_back();
    }
    if (characters.size() == 0) characters.push_back('0');

    *this = BigInteger(characters);
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    if (other.size() > size()) return *this = 0;
    
    bool old_negative = negative;
    BigInteger result = 0;
    
    auto substracted = other;
    substracted.shift(size() - other.size());
    substracted.negative = negative;
    
    for (size_t offset = size() - other.size(); offset + 1 > 0; --offset) {
        result.shift(1);
        while(compare_absolute(substracted) >= 0) {
            *this -= substracted;
            ++result;
        }
        substracted.shift(-1);
    }
    
    result.negative = result.is_zero() ? false : (other.negative ^ old_negative);
    return *this = result;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    auto diff = ((*this) / other);
    
    diff *= other;
    return *this -= diff;
}

BigInteger& BigInteger::operator++() {
    resolve_carry(negative ? -1 : 1, 0);
    resolve_sign();
    clear_leading_zeroes(digits);
    return *this;
}

BigInteger BigInteger::operator++(int) {
    auto result = *this;
    ++*this;
    return result;
}

BigInteger& BigInteger::operator--() {
    resolve_carry(negative ? 1 : -1, 0);
    resolve_sign();
    clear_leading_zeroes(digits);
    return *this;
}

BigInteger BigInteger::operator--(int) {
    auto result = *this;
    --*this;
    return result;
}

BigInteger BigInteger::operator-() const {
    auto result = *this;
    result.negative = !negative && !is_zero();
    return result;
}

string BigInteger::toString() const {
    size_t offset = negative ? 1 : 0;
    string result(size() + offset, '\0');
    for (size_t i = 0; i < size(); ++i) {
        result[result.size() - i - 1] = digit_to_char(digits[i]);
    }
    if (negative) result[0] = '-';
    return result;
}

BigInteger::operator long long() const {
    long long result = 0;
    long long power = 1;
    for(size_t i = 0; i < size(); ++i) {
        result += digits[i] * power;
        power *= BASE;
    }
    if (is_negative()) result *= -1;

    return result;
}

BigInteger::operator bool() const {
    return !is_zero();
}

bool BigInteger::is_negative() const {
    return negative;
}

void BigInteger::invert_sign() {
    if (is_zero()) return;

    negative = !negative;
}

BigInteger BigInteger::power(const BigInteger& indicator, const BigInteger& exponent) {
    BigInteger current_power = indicator;
    BigInteger result = 1;
    for (auto digit : exponent.digits) {
        for (auto t = 0; t < digit; ++t) {
            result *= current_power;
        }
        BigInteger new_power = current_power;
        for (auto t = 1; t < BASE; ++t) {
            new_power *= current_power;
        }
        current_power = new_power;
    }
    return result;
}

bool operator==(const BigInteger& left, const BigInteger& right) {
    if (left.size() != right.size()) return false;
    if (left.is_negative() != right.is_negative()) return false;
    return (left <=> right) == strong_ordering::equivalent;
}

bool operator!=(const BigInteger& left, const BigInteger& right) {
    return !(left == right);
}

BigInteger operator+(const BigInteger& left, const BigInteger& right) {
    BigInteger result = left;
    return result += right;
}

BigInteger operator-(const BigInteger& left, const BigInteger& right) {
    BigInteger result = left;
    return result -= right;
}

BigInteger operator*(const BigInteger& left, const BigInteger& right) {
    BigInteger result = left;
    return result *= right;
}

BigInteger operator/(const BigInteger& left, const BigInteger& right) {
    BigInteger result = left;
    return result /= right;
}

BigInteger operator%(const BigInteger& left, const BigInteger& right) {
    BigInteger result = left;
    return result %= right;
}

std::istream& operator>>(std::istream& input, BigInteger& value) {
    string data;
    input >> data;
    value = BigInteger(data);
    return input;
}

std::ostream& operator<<(std::ostream& output, const BigInteger& value) {
    output << value.toString();
    return output;
}

strong_ordering operator<=>(const BigInteger& left, const BigInteger& right) {
    if (left.is_negative() && !right.is_negative()) return strong_ordering::less;
    if (!left.is_negative() && right.is_negative()) return strong_ordering::greater;
    if (left.is_negative()) {
        return right.compare_absolute(left);
    } else {
        return left.compare_absolute(right);
    }
}

BigInteger operator""_bi(unsigned long long source) {
    BigInteger result = static_cast<long long>(source / 2);
    result *= 2;
    result += static_cast<int>(source % 2);
    return result;
}

BigInteger gcd(BigInteger left, BigInteger right) {
    BigInteger* big = &left;
    BigInteger* small = &right;

    while(!small->is_zero()) {
        *big %= *small;
        std::swap(small, big);
    }
    return *big;
}

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
    // answer * power(10, precision) is around numerator / denominator
    // so let's calculate it this way
    // use precision + 1 to do correct round up/down
    BigInteger additional = BigInteger::power(10, precision + 1);

    // pre_divided is almost what we need. now only do correct round and divide by 10
    BigInteger pre_divided = ((numerator * additional) / denominator);
    if (pre_divided.is_negative()) pre_divided.invert_sign();

    // divided is the answer string except for decimal dot
    string divided = ((pre_divided + 5) / 10).toString();
    string result = "";
    if (numerator.is_negative()) result = "-";

    // now add the dot in the correct place
    if (divided.size() <= precision) {
        result += "0.";
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
    // double is 52 bits of mantissa and 11 bits of exponent
    // first using bin_search find the best exponent
    long long exponent_l = -1024;
    long long exponent_r = 1024;
    while(exponent_l != exponent_r - 1) {
        long long m = (exponent_l + exponent_r) / 2;
        
        BigInteger result = to_int_binary_shifted(m);
        if (result.is_negative()) result.invert_sign();

        if (result >= DOUBLE_MAX) {
            exponent_r = m;
        } else {
            exponent_l = m;
        }
    }
    // then save the answer as double
    auto mantissa = to_int_binary_shifted(exponent_l);    
    return ldexp(static_cast<long long>(mantissa), -exponent_l);
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

