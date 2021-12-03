#pragma once

#include <cpp-httplib/httplib.h>
#include <jsonrpccxx/common.hpp>
#include <jsonrpccxx/iclientconnector.hpp>
#include <string>

namespace sdk
{
    class http_connector : public jsonrpccxx::IClientConnector
    {
    public:
        http_connector(const std::string& host, int port) : http_client_(host.c_str(), port) {}

        auto Send(const std::string& request) -> std::string override
        {
            auto res = http_client_.Post("/", request, "application/json");
            if (!res || res->status != 200)
            {
                throw jsonrpccxx::JsonRpcException(-32003, "client connector error, received status != 200");
            }
            return res->body;
        }

    private:
        httplib::Client http_client_;
    };

} // namespace sdk
