#pragma once
#include <compare>
#include <complex>
#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <math.h>

using std::vector;
using std::string;
using std::strong_ordering;

using digit_t = int;
using complex = std::complex<double>;

size_t revert_binary(size_t index, size_t length);

class BigInteger {
  private:
    static const digit_t SYMBOL_BASE = 10;
    static const digit_t DIGIT_BASE = 1000;
    static const size_t BASE_LEN = 3;
    vector<digit_t> digits;
    bool negative = false;

    // REWRITE IF BASE CHANGES!!!!
    static digit_t string_to_digit(const string& source, size_t from, size_t to);
    static string digit_to_string(digit_t d);

    // contract: reduced is bigger than subtracted
    static void subtract_vectors(const vector<digit_t>& reduced, const vector<digit_t>& subtracted, vector<digit_t>& difference);
  
    // Applies Fast Fourier Transform (or it's inversed form) to the vector of coefficients (or values respectively)
    static void fft(vector<complex>& coefficients, bool inversed=false);
    
    static vector<digit_t> complex_to_digits(const vector<complex>& values);

    static vector<complex> digits_to_complex(const vector<digit_t>& values, size_t target_size); 

    static void clear_leading_zeroes(vector<digit_t>& digits);

    static void reorder_for_fft(vector<complex>& source);


    void add_absolute(const BigInteger& other);

    void subtract_absolute(const BigInteger& other);

    strong_ordering compare_absolute(const BigInteger& other) const;

    void resolve_carry(digit_t carry, size_t index);

    void resolve_sign();

    BigInteger multiplied_by_digit(digit_t value) const;

    static std::pair<BigInteger, BigInteger> divide(const BigInteger& numerator, const BigInteger& denominator);
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

    BigInteger operator++(digit_t);

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

digit_t BigInteger::string_to_digit(const string& source, size_t from, size_t to) {
    digit_t result = 0;
    for (size_t i = from; i < to; ++i) {
        result *= SYMBOL_BASE;
        result += source[i] - '0';
    }
    return result;
}

string BigInteger::digit_to_string(digit_t d) {
    string result(BASE_LEN, '0');
    for(size_t i = 0; i < BASE_LEN; ++i) {
        result[BASE_LEN - 1 - i] = '0' + static_cast<char>(d % SYMBOL_BASE);
        d /= SYMBOL_BASE;
    }
    return result;
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

void BigInteger::resolve_carry(digit_t carry, size_t index) {
    while(carry != 0) {
        if (index == size()) digits.push_back(0);
        carry += digits[index];
        digits[index] = carry % DIGIT_BASE;
        if (carry < 0) {
            digits[index] += DIGIT_BASE;
            carry /= DIGIT_BASE;
            --carry;
        } else {
            carry /= DIGIT_BASE;
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
        carry = digits[i] / DIGIT_BASE;
        digits[i] %= DIGIT_BASE;
    }
    resolve_carry(carry, other.size());
}

void BigInteger::subtract_vectors(const vector<digit_t>& reduced, const vector<digit_t>& subtracted, vector<digit_t>& difference) { 
    difference.reserve(reduced.size());

    digit_t carry = 0;
    for (size_t i = 0; i < reduced.size(); ++i) {
        if (i == difference.size()) difference.push_back(0);
                
        digit_t to_subtract = (i < subtracted.size()) ? subtracted[i] : 0;
        
        difference[i] = reduced[i] - carry - to_subtract;
        carry = 0;
        if (difference[i] < 0) {
            difference[i] += DIGIT_BASE;
            carry = 1;
        }

        if (i >= subtracted.size() && carry == 0) {
            if (&difference == &reduced) break;
        }
    }

    clear_leading_zeroes(difference);
}

void BigInteger::subtract_absolute(const BigInteger& other) {
    auto comparison_result = compare_absolute(other);
    
    if (comparison_result == strong_ordering::less) {
        subtract_vectors(other.digits, digits, digits);
        negative = !negative;
    } else if(comparison_result == strong_ordering::equivalent) {
        digits.clear();
        digits.push_back(0);
        negative = false;
    } else {
        subtract_vectors(digits, other.digits, digits);
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
        digits.push_back(static_cast<digit_t>(value % DIGIT_BASE));
        value /= DIGIT_BASE;
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
    size_t digits_count = 1 + (source.size() - offset - 1) / BASE_LEN;
    digits.resize(digits_count);
    for (size_t i = 0; i < digits_count; ++i) {
        digit_t block_begin = static_cast<digit_t>(source.size()) - static_cast<digit_t>((i + 1) * BASE_LEN);
        size_t end_index = source.size() - i * BASE_LEN;
        size_t begin_index = static_cast<size_t>(std::max(static_cast<digit_t>(offset), block_begin));
        digits[i] = string_to_digit(source, begin_index, end_index);
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
        subtract_absolute(other);
    }
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    if (negative == other.negative) {
        subtract_absolute(other);
    } else {
        add_absolute(other);
    }
    return *this;
}

vector<complex> BigInteger::digits_to_complex(const vector<digit_t>& values, size_t target_size) {
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
    long long carry = 0;
    for (size_t i = 0; i < result.size(); ++i) {
        carry += static_cast<long long>(values[i].real() + 0.5);
        result[i] = carry % DIGIT_BASE;
        carry /= DIGIT_BASE;
    }
    
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
        long double angle = 2 * M_PI / static_cast<long double>(block_length) * (inversed ? -1 : 1);
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
    digits = complex_to_digits(my_complex_values);
    negative ^= other.negative;
    if (is_zero() || other.is_zero()) negative = false;
    return *this;
}

BigInteger BigInteger::multiplied_by_digit(digit_t value) const {
    digit_t carry = 0;
    BigInteger result = *this;
    for (size_t i = 0; carry > 0 || i < size(); ++i) {
        if (i < size()) {
            carry += value * digits[i];
        } else {
            result.digits.push_back(0);
        }
        result.digits[i] = carry % DIGIT_BASE;
        carry /= DIGIT_BASE;
    }
    clear_leading_zeroes(result.digits);
    result.resolve_sign();
    return result;
}

void BigInteger::shift(int shift_value) {
    if (-shift_value >= static_cast<int>(size())) {
        *this = 0;
        return;
    }
    size_t new_size = size() + static_cast<size_t>(shift_value);
    vector<digit_t> new_digits(new_size, 0);
    for (size_t i = static_cast<size_t>(std::max(0, shift_value)); i < new_digits.size(); ++i) {
        new_digits[i] = digits[static_cast<size_t>(static_cast<int>(i) - shift_value)];
    }
    swap(digits, new_digits);
    clear_leading_zeroes(digits);
    resolve_sign();
}

std::pair<BigInteger, BigInteger> BigInteger::divide(const BigInteger& numerator, const BigInteger& denominator) {
    if (denominator.size() > numerator.size()) return std::make_pair(0, numerator);
    BigInteger result = 0;
    BigInteger mod = numerator;

    auto subtracted = denominator;
    subtracted.shift(static_cast<int>(numerator.size() - denominator.size()));
    subtracted.negative = numerator.negative;
    for (size_t offset = numerator.size() - denominator.size(); offset + 1 > 0; --offset) {
        result.shift(1);
        digit_t left = 0, right = DIGIT_BASE;
        while(left != right - 1) {
            digit_t m = (left + right) / 2;
            if (subtracted.multiplied_by_digit(m).compare_absolute(mod) > 0) {
                right = m;
            } else {
                left = m;
            }
        }
        mod -= subtracted.multiplied_by_digit(left);
        result += left;
        subtracted.shift(-1);
    }
    result.negative = result.is_zero() ? false : (denominator.negative ^ numerator.negative);
    return std::make_pair(result, mod);
   
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    return *this = divide(*this, other).first;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    return *this = divide(*this, other).second;
}

// There could be *this += 1, but as my tests showed it works very-very slow
// Because it has to create a new BigInteger from 1 (therefore create a new vector<digit_t>)
// So it was about 1e6 iterations per second of ++
// That is why there is more complicated code
BigInteger& BigInteger::operator++() {
    resolve_carry(negative ? -1 : 1, 0);
    resolve_sign();
    clear_leading_zeroes(digits);
    return *this;
}

BigInteger BigInteger::operator++(digit_t) {
    auto result = *this;
    ++*this;
    return result;
}

//The same as with operator++
BigInteger& BigInteger::operator--() {
    if (*this == 0) return *this = -1;
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
    string result(size() * BASE_LEN + offset, '0');
    for (size_t i = 0; i < size(); ++i) {
        string additional = digit_to_string(digits[i]);
        for (size_t j = 0; j < BASE_LEN; ++j) {
            result[i * BASE_LEN + j] = additional[BASE_LEN - 1 - j];
        }
    }
    while(result.size() > 1 && result[result.size() - offset - 1] == '0') result.pop_back();

    if (negative) result[result.size() - 1] = '-';
    reverse(result.begin(), result.end());
    
    return result;
}

BigInteger::operator long long() const {
    long long result = 0;
    long long power = 1;
    for(size_t i = 0; i < size(); ++i) {
        result += digits[i] * power;
        power *= DIGIT_BASE;
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
        for (auto t = 1; t < DIGIT_BASE; ++t) {
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
    result += static_cast<digit_t>(source % 2);
    return result;
}

BigInteger gcd(BigInteger left, BigInteger right) {
    if (left.is_negative()) left.invert_sign();
    if (right.is_negative()) right.invert_sign();
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

    BigInteger to_digit_t_binary_shifted(long long binary_shift) const;
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
    if (denominator.is_negative()) {
        numerator.invert_sign();
        denominator.invert_sign();
    }
    if (to_reduct == 1) return;
    numerator /= to_reduct;
    denominator /= to_reduct; 
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
    BigInteger additional = BigInteger::power(10, static_cast<long long>(precision + 1));
    // pre_divided is almost what we need. now only do correct round and divide by 10
    BigInteger pre_divided = (numerator * additional);
    pre_divided /= denominator;
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

BigInteger Rational::to_digit_t_binary_shifted(long long binary_shift) const {
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
    static const size_t double_precision =  300;
    return std::stod(asDecimal(double_precision));
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

