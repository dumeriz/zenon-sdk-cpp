#pragma once

#include "sdk.hpp"

#include <functional>
#include <jsonrpccxx/client.hpp>
#include <memory>
#include <mutex>
#include <type_traits>

// internal usage only
namespace sdk::detail
{
    /// @brief wraps managed pointers to an api connector and the jsonrpc library instance.
    class api_connection
    {
    public:
        /// @brief call the specified api method with a list of unnamed parameters (that will be converted to a
        /// parameter array).
        /// @param id is a unique id for that method, either an integer or a string.
        /// @param method is the name of the method name.
        template <typename Ret, typename Id, typename... Args>
        auto call(Id id, std::string_view method, Args&&... args) -> Ret
        {
            static_assert(std::is_same_v<Id, int> || std::is_convertible_v<Id, std::string>);

            std::vector<nlohmann::json> const params{std::forward<Args>(args)...};

            // prevent mutation of the connector via _set_connector while in use
            std::lock_guard<std::mutex> lock(mtx_);
            return client_->CallMethod<Ret>(id, method.data(), params);
        }

        /// @brief call the specified api method with a map of named parameters.
        /// @param id is a unique id for that method, either an integer or a string.
        /// @param method is the name of the method name.
        template <typename Ret, typename Id>
        auto call(Id id, std::string_view method, std::map<std::string, nlohmann::json> const& params) -> Ret
        {
            static_assert(std::is_same_v<Id, int> || std::is_convertible_v<Id, std::string>);

            // prevent mutation of the connector via _set_connector while in use
            std::lock_guard<std::mutex> lock(mtx_);
            return client_->CallMethod<Ret>(id, method.data(), params);
        }

    private:
        friend auto sdk::set_connector(sdk::connector_t) -> void;
        auto _set_connector(sdk::connector_t);

        std::mutex mtx_;
        sdk::connector_t connector_;
        std::unique_ptr<jsonrpccxx::JsonRpcClient> client_;
    };
} // namespace sdk::detail
