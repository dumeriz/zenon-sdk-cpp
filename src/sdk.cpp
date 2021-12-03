#include "sdk.hpp"

#include "internal.hpp"

sdk::detail::api_connection connection;

auto
sdk::detail::api_connection::_set_connector(sdk::connector_t connector)
{
    std::lock_guard<std::mutex> lock(mtx_);
    connector_ = connector;
    client_    = std::make_unique<jsonrpccxx::JsonRpcClient>(*connector_, jsonrpccxx::version::v2);
}

auto
sdk::set_connector(sdk::connector_t connector) -> void
{
    connection._set_connector(connector);
}
