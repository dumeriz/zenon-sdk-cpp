#include "sdk.hpp"

#include "internal.hpp"

sdk::detail::api_connection connection;

auto sdk::detail::api_connection::_set_connector(sdk::connector_t connector)
{
    std::lock_guard<std::mutex> lock(mtx_);
    connector_ = connector;
    client_.reset();
    if (connector_)
    {
        client_ = std::make_unique<jsonrpccxx::JsonRpcClient>(*connector_, jsonrpccxx::version::v2);
    }
}

auto sdk::detail::set_connector(sdk::connector_t connector) -> void { connection._set_connector(connector); }

auto sdk::connected() -> bool { return connection.connected(); }
