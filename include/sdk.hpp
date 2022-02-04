#pragma once

#include "connection_error.hpp"
#include "connector.hpp"
#include <memory>

namespace sdk
{
    using connector_t = std::shared_ptr<connector>;

    namespace detail
    {
        auto set_connector(connector_t connector) -> void;
    }

    /// @brief Sets the connector object that is used to communicate with the json api.
    template <typename C, typename... Args> auto set_connector(Args&&... args) -> void
    {
        // make sure that we do not hold on to a previous connection accidentally
        detail::set_connector(nullptr);
        try
        {
            detail::set_connector(std::make_shared<C>(std::forward<Args>(args)...));
        }
        catch (connection_error const& ce)
        {
            throw ce;
        }
        catch (...)
        {
            throw connection_error{"Failed to establish a connection: Unknown reason"};
        }
    }

    /// @brief Checks if a connection has been established.
    auto connected() -> bool;

} // namespace sdk
