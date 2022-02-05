#pragma once

#include <jsonrpccxx/iclientconnector.hpp>

namespace sdk
{
    struct connector : public jsonrpccxx::IClientConnector
    {
    public:
        ~connector() override = default;
        virtual auto connected() const -> bool = 0;
    };
} // namespace sdk
