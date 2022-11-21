#include <limits>
#include "biginteger.h"

size_t BigInteger::size() const {
    return digits.size();
}

static digit_t BigInteger::char_to_digit(char c) {
    if (c < '0' || c >= ('0' + BASE)) throw InvalidInputException(c);
    return c - '0';
}

static char BigInteger::digit_to_char(digit_t d) {
    return '0' + d;
}

bool BigInteger::is_zero() const {
    return size() == 0 && digits[0] == 0;
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

void BigInteger::add_absolute(const BigInteger& other) {
    digit_t carry = 0;
    digits.reserve(other.size());

    for (size_t i = 0; i < other.size(); ++i) {
        if (i == size()) digits.push_back(0);
        digits[i] += other.digits[i];
        digits[i] += carry;
        carry = digits[i] / BASE;
        digits[i] %= BASE;
    }
    if (carry > 0) digits.push_back(carry);
}

static void BigInteger::substract_vectors(const vector<digit_t>& reduced, const vector<digit_t>& substracted, vector<digit_t>& difference) {
    assert(substracted.size() <= reduced.size());
    
    difference.reserve(reduced.size());

    digit_t carry = 0;
    for (size_t i = 0; i < reduced.size(); ++i) {
        if (i == difference.size()) difference.push_back(0);

        difference[i] = reduced[i] - carry;
        if (i < substracted.size()) {
            difference[i] -= substracted[i];
        }

        carry = 0;
        if (difference[i] < 0) {
            difference[i] += BASE;
            carry = 1;
        }
    }

    while(difference.size() > 1 && difference.back() == 0) difference.pop_back();
    
    assert(carry == 0);
}

void BigInteger::substract_absolute(const BigInteger& other) {
    switch(compare_absolute(other)) {
      case strong_ordering::less {
        substract_vectors(other.digits, digits, digits);
        negative = !negative;
        break;
      }
      case strong_ordering::equivalent {
        digits.clear();
        digits.push_back(0);
        negative = false;
        break;
      }
      case strong::ordering::greater {
        substract_vectors(digits, other.digits, digits);
        break;
      }
    };
}

BigInteger::BigInteger() : BigInteger(0) {}

BigInteger::BigInteger(long long value) {
    if (value < 0) {
        negative = true;
        value = -value;
    }
    do {
        digits.push_back(value % BASE);
        value /= BASE;
    } while(value > 0);
}

BigInteger::BigInteger(unsigned long long value) : BigInteger(static_cast<long long>(value / 2) {
    (*this) *= 2;
    (*this) += value % 2;
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
    if (negative != other.negative) {
        add_absolute(other);
    } else {
        substract_absolute(other);
    }
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    if (negative != other.negative) {
        substract_absolute(other);
    } else {
        add_absolute(other);
    }
    return *this;
}

static vector<complex> BigIntger::digits_to_complex(const vector<digit_t>& values, size_t target_size) {
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

static vector<digit_t> BigInteger::complex_to_digits(const vector<complex>& values) {
    vector<digit_t> result(values.size(), 0);
    int carry = 0;
    for (size_t i = 0; i < result.size(); ++i) {
        carry + = static_cast<digit_t>(values[i].real() + 0.5);
        result[i] = carry % BASE;
        carry /= BASE;
    }

    // since values is built with enough space (on the initialization of fft),
    // it should always contain enough zero digits to contain all the carry
    assert(carry == 0);
    
    while(result.size() > 1 && result.back() == 0) result.pop_back();

    return result;
}

static void BigInteger::fft(vector<complex>& source, bool inversed) {
    complex root_coefficient = complex(-1.0, inversed ? -0.0 : 0.0);

    for (size_t block_length = 2; block_length <= source.size(); block_length *= 2) {
        for (size_t block_id = 0; block_id < source.size() / block_length; ++block_id) {
            size_t left_part = block_id * block_length;
            size_t right_part = left_part + block_length / 2;
            for (size_t i = 0; i < block_length / 2; ++i) {
                complex first = source[left_part + i] + root_coefficient * source[right_part + i];
                complex second = source[left_part + i] - root_coefficient * source[right_part + i];
                source[left_part + i] = first;
                source[right_part + i] = second;
            } 
        }
        root_coefficient = std::sqrt(root_coefficient);
    }
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    auto my_complex_values = digits_to_complex(digits, size() + other.size());
    fft(my_complex_values);
     
    auto other_complex_values = digits_to_complex(other.digits, size() + other.size());
    fft(other_complex_values);

    for (int i = 0; i < my_complex_values.size(); ++i) {
        my_complex_values[i] *= other_complex_values[i];
    }

    fft(my_complex_values, true);

    digits.swap(complex_to_digits(my_complex_values));
    negative ^= other.negative;
    if (is_zero() || other.is_zero()) negative = false;

    return *this;
}

BigInteger BigInteger::shift(int digits) const {
    string characters = toString();
    for (int i = 0; i < digits; ++i) {
        characters.push_back('0');
    }
    for (int i = digits; i < 0 && characters.size(); ++i) {
        characters.pop_back();
    }
    if (characters.size() == 0) characters.push_back('0');

    return BigInteger(characters);
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    if (other.size() > size()) return *this = 0;

    BigInteger result = 0;
    auto substracted = other.shift(size() - other.size());
    substracted.negative = negative;
    for (size_t offset = size() - other.size(); offset != -1; --offset) {
        result = result.shift(1);
        while(*this > substracted) {
            *this -= substracted;
            ++result;
        }
        substracted.shift(-1);
    }
    result.negative ^= other.negative;
    return *this = result;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    auto diff = ((*this) / other) * other;
    return *this -= diff;
}

BigInteger& BigInteger::operator++() {
    return *this += 1;
}

BigInteger BigInteger::operator++(int) {
    auto result = *this;
    ++*this;
    return result;
}

BigInteger BigInteger::operator-() const {
    auto result = *this;
    result.negative = !negative && !is_zero();
}

string BigInteger::toString() const {
    size_t offset = negative ? 1 : 0;
    string result(size() + (negative ? 1 : 0));
    for (size_t i = 0; i < size; ++i) {
        result[result.size() - i - 1] = digit_to_char(digits[i])
    }
    if (negative) result[0] = '-';
    return result;
}

BigInteger::operator long long() const {
    BigInteger longMin = static_cast<BigInteger>(std::numeric_limits<long long>::min());
    BigInteger longMax = static_cast<BigInteger>(std::numeric_limits<long long>::max());
    if (*this < longMin || longMax < *this) throw TooBigCastException(*this, typeid(long long));

    long long result = 0;
    long long power = 1;
    for(size_t i = 0; i < size(); ++i) {
        result += digits[i] * power;
        power *= BASE;
    }
    if (is_negative()) result *= -1;

    return result;
}

BigInteger::operator unsigned long long() const {
    if (is_negative()) throw NegativeToUnsignedCastException(*this, typeid(unsigned long long));
 
    BigInteger longMin = static_cast<BigInteger>(std::numeric_limits<long long>::min());
    BigInteger longMax = static_cast<BigInteger>(std::numeric_limits<long long>::max());
    if (*this < longMin || longMax < *this) throw TooBigCastException(*this, typeid(long long));
 
    unsigned long long result = 0;
    unsigned long long power = 1;
    for(size_t i = 0; i < size(); ++i) {
        result += digits[i] * power;
        power *= BASE;
    }

    return result;    
}

BigInteger::operator bool() const {
    return !is_zero();
}

bool BigInteger::is_negative() const {
    return negative;
}

void invert_sign() {
    if (is_zero()) return;

    negative = !negative;
}

BigInteger::~BigInteger() {}

bool operator==(const BigInteger& left, const BigInteger& right) {
    if (left.size() != right.size()) return false;
    if (left.is_negative() != right.is_negative()) return false;
    return (left <=> right) == strong_ordering::equivalent;
}

bool operator!=(const BigInteger& left, const BigInteger& right) {
    return !(*this == other);
}

BigInteger operator+(const BigInteger& left, const BigInteger& right) {
    auto result = left;
    return result += right;
}

BigInteger operator-(const BigInteger& left, const BigInteger& right) {
    auto result = left;
    return result -= right;
}

BigInteger operator*(const BigIntegr& left, const BigInteger& right) {
    auto result = left;
    return result *= right;
}

BigInteger operator/(const BigInteger& left, const BigIntegr& right) {
    auto result = left;
    return result /= right;
}

BigInteger operator%(const BigIntegr& left, const BigInteger& right) {
    auto result = left;
    return result %= right;
}

std::istream& operator>>(std::istream& input, BigInteger& value) {
    string data;
    input >> data;
    value = data;
}

std::ostream& operator<<(std::ostream& output, const BigInteger& value) {
    output << value.toString();
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

BigInteger operator""_bi(long long source) {
    BigInteger result = source;
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

