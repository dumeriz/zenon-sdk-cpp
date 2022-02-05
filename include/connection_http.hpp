#pragma once

#include "connection_error.hpp"
#include "connector.hpp"

#include <cpp-httplib/httplib.h>
#include <string>

namespace sdk
{
    class http_connector : public connector
    {
    public:
        http_connector(const std::string& host, int port) : http_client_(host.c_str(), port)
        {
            if (!http_client_.is_valid())
            {
                throw connection_error{"Failed to initialize client"};
            }

            if (auto result{validate_connection()}; !result.first)
            {
                throw connection_error{"Initial request for frontier momentum failed: got " + result.second};
            }
            connected_.store(true);
        }

        ~http_connector() override = default;

        auto Send(std::string const& request) -> std::string override
        {
            if (!connected())
            {
                throw connection_error{"Not connected"};
            }

            return exchange_data(request);
        }

        auto connected() const -> bool override { return connected_.load(); }

    private:
        httplib::Client http_client_;
        std::atomic_bool connected_{};

        // quick validation that we actually can exchange data with the node.
        auto validate_connection() -> std::pair<bool, std::string>
        {
            auto const response{
                exchange_data("{\"jsonrpc\":\"2.0\",\"id\":31,\"method\":\"ledger.getFrontierMomentum\"}")};
            return std::make_pair(response.find("result") != std::string::npos, response);
        }

        auto exchange_data(std::string const& request) -> std::string
        {
            auto const res = http_client_.Post("/", request, "application/json");
            if (!res)
            {
                connected_.store(false);
                throw connection_error{"Error in POST: " + std::to_string(res.error())};
            }
            else if (res->status != 200)
            {
                // todo: other error type
                throw connection_error{"Unexpected response; status=" + std::to_string(res->status)};
            }
            return res->body;
        }
    };

} // namespace sdk
