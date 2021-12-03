#pragma once

#include <jsonrpccxx/iclientconnector.hpp>
#include <memory>

namespace sdk
{
    using connector_t = std::shared_ptr<jsonrpccxx::IClientConnector>;

    /// @brief Sets the connector object that is used to communicate with the json api.
    /// @param connector holds an object satisfying the jsonrpccxx::IClientConnector interface.
    auto set_connector(connector_t connector) -> void;
} // namespace sdk
