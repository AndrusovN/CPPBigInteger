#pragma once

#include <exception>
#include <iostream>

char* rebuild_c_string_from_string(const string& source) {
    char* result = new char[source.size() + 1]();
    std::copy(source.c_str(), source.c_str() + source.size(), result);
    return result;
}

class InvalidInputException: public std::exception {
  private:
    string input;

  public:
    InvalidInputException(string input) : input(input) {}
   
   const char* what() const noexcept override {
        auto data = "Invalid input to create BigInteger: " +
            input + ". Expected a numerical literal";
        return rebuild_c_string_from_string(data);
    }
};

class DivisionByZeroException: public std::exception {
  private:
    BigInteger value;
  public:
    DivisionByZeroException(const BigInteger& value) : value(value) {}

    const char* what() const noexcept override {
        auto data = "Division by zero! Trying to divide " + value.toString() + " by zero";
        return rebuild_c_string_from_string(data);
    } 
};

class CastException: public std::exception {
  private:
    BigInteger value;
    std::type_info cast_type;
  protected:
    virtual string const exception_message() {
        return "Exception during cast. Value to cast: " + value.toString() + " type to cast: " + cast_type.name();

    }
  public:
    CastException(const BigInteger& value, std::type_info cast_type) : value(value), cast_type(cast_type) {}

    const char* what() const noexcept override {
        return rebuild_c_string_from_string(exception_message());
    }
};

class TooBigCastException: public CastException {
  protected:
    string exception_message() const override {
      return "Too big cast. " + CastException::exception_message();
    }
  public:
    TooBigCastException(const BigInteger& value, std::type_info cast_type) : CastException(value, cast_type) {}

    const char* what() const noexcept override {
        return rebuild_c_string_from_string(exception_message());
    }
};

class NegativeToUnsignedCastException: public CastException {
  protected:
    string exception_message() const override {
        return "Trying to cast negative value to unsigned type. " + CastException::exception_message();
    }  
  public:
    NegativeToUnsignedCastException(const BigInteger& value, std::type_info cast_type) : CastException(value, cast_type) {}

    const char* what() const noexcept override {
        return rebuild_c_string_from_string(exception_message());
    }  
};

