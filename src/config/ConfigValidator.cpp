#include "ConfigValidator.hpp"

ConfigValidator::ConfigValidator()
{
}

ConfigValidator::~ConfigValidator()
{
}

ConfigValidator::ConfigValidator(ConfigValidator const &other)
{
    *this = other;
}

ConfigValidator &ConfigValidator::operator=(ConfigValidator const &other)
{
    if (this != &other)
    {
    }
    return *this;
}
