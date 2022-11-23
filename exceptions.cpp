#include "exceptions.h"
char* rebuild_c_string_from_string(const string& source) {
    char* result = new char[source.size() + 1]();
    std::copy(source.c_str(), source.c_str() + source.size(), result);
    return result;
}

InvalidInputException::InvalidInputException(string input) : input(input) {}

const char* InvalidInputException::what() const noexcept {
    auto data = "Invalid input to create BigInteger: " +
        input + ". Expected a numerical literal";
    return rebuild_c_string_from_string(data);
}

DivisionByZeroException::DivisionByZeroException(const BigInteger& value) : value(value) {}

const char* DivisionByZeroException::what() const noexcept {
    auto data = "Division by zero! Trying to divide " + value.toString() + " by zero";
    return rebuild_c_string_from_string(data);
}

string CastException::exception_message() const {
    return "Exception during cast. Value to cast: " + value.toString() + " type to cast: " + cast_type.name();
} 
CastException::CastException(const BigInteger& value, const std::type_info& cast_type) : value(value), cast_type(cast_type) {}

const char* CastException::what() const noexcept {
    return rebuild_c_string_from_string(exception_message());
}

string TooBigCastException::exception_message() const {
    return "Too big cast. " + CastException::exception_message();
}

TooBigCastException::TooBigCastException(const BigInteger& value, const std::type_info& cast_type) : CastException(value, cast_type) {}

string NegativeToUnsignedCastException::exception_message() const {
    return "Trying to cast negative value to unsigned type. " + CastException::exception_message();
}  

NegativeToUnsignedCastException::NegativeToUnsignedCastException(const BigInteger& value, const std::type_info& cast_type) : CastException(value, cast_type) {}

