#pragma once

#include <stdexcept>
namespace sdk
{
    class connection_error : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };
} // namespace sdk
