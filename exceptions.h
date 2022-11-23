#pragma once

#include <exception>
#include <iostream>

#include "biginteger.h"

using std::string;

char* rebuild_c_string_from_string(const string& source);

class InvalidInputException: public std::exception {
  private:
    string input;

  public:
    InvalidInputException(string input);
   
   const char* what() const noexcept override;
};

class DivisionByZeroException: public std::exception {
  private:
    BigInteger value;
  public:
    DivisionByZeroException(const BigInteger& value);
    const char* what() const noexcept override;
};

class CastException: public std::exception {
  private:
    BigInteger value;
    const std::type_info& cast_type;
  protected:
    virtual string exception_message() const;
  public:
    CastException(const BigInteger& value, const std::type_info& cast_type);

    const char* what() const noexcept override;
};

class TooBigCastException: public CastException {
  protected:
    string exception_message() const override;
  public:
    TooBigCastException(const BigInteger& value, const std::type_info& cast_type);
};

class NegativeToUnsignedCastException: public CastException {
  protected:
    string exception_message() const override;
  public:
    NegativeToUnsignedCastException(const BigInteger& value, const std::type_info& cast_type);
};

