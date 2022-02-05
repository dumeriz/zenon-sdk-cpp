#pragma once

#include "connection_error.hpp"
#include "connector.hpp"

#include <nlohmann/json.hpp>

#include <memory>

namespace sdk
{
    using connector_t = std::shared_ptr<connector>;
    using json_t = nlohmann::json;

    template <typename T> struct return_t
    {
        bool const ok;
        T const value;
        operator bool() const { return ok; }
    };

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

    /// @brief Returns the error from the last api call.
    auto error() -> std::string const&;

} // namespace sdk
