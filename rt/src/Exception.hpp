#pragma once
#include <stdexcept>
//typedef std::logic_error Exception;
#include "String.hpp"

class Exception : public std::logic_error {
public:
    explicit Exception(const String& arg);
};
