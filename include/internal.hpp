#pragma once

#include "sdk.hpp"

#include <functional>
#include <jsonrpccxx/client.hpp>
#include <memory>
#include <mutex>

// internal usage only
namespace sdk::detail
{
    /// @brief wraps managed pointers to an api connector and the jsonrpc library instance.
    class api_connection
    {
    public:
        /// @brief call the specified api method.
        /// @param id is a unique id for that method.
        /// @param method is the name of the method name.
        template <typename Ret, typename... Args> auto call(int id, std::string_view method, Args&&... args) -> Ret
        {
            // prevent mutation of the connector via _set_connector while in use
            std::lock_guard<std::mutex> lock(mtx_);
            return client_->CallMethod<Ret>(id, method.data(), std::forward<Args>(args)...);
        }

        /// @brief call the specified api method.
        /// @param id is a unique id for that method.
        /// @param method is the name of the method name.
        template <typename Ret, typename... Args>
        auto call(std::string const& id, std::string_view method, Args&&... args) -> Ret
        {
            // prevent mutation of the connector via _set_connector while in use
            std::lock_guard<std::mutex> lock(mtx_);
            return client_->CallMethod<Ret>(id, method.data(), std::forward<Args>(args)...);
        }

    private:
        friend auto sdk::set_connector(sdk::connector_t) -> void;
        auto _set_connector(sdk::connector_t);

        std::mutex mtx_;
        sdk::connector_t connector_;
        std::unique_ptr<jsonrpccxx::JsonRpcClient> client_;
    };
} // namespace sdk::detail
