#pragma once

#include "config.hpp"
#include "connection_error.hpp"
#include "connection_http.hpp"
#include "connection_ws.hpp"
#include "connection_wss.hpp"
#include "sdk.hpp"

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

// ----- Helper functions -----------------------------------------------

// connection parameters declared in config.hpp, set in main.cpp
auto inline set_http_connection() { sdk::set_connector<sdk::http_connector>(http_address, http_port); }
auto inline set_ws_connection() { sdk::set_connector<sdk::ws_connector>(ws_address, ws_port); }
auto inline set_wss_connection() { sdk::set_connector<sdk::wss_connector>(wss_address, wss_port); }

// ----- Macro to simplify test executions for all endpoints ---------

#define DEF_TEST(label, topic, test_method) \
    TEST_CASE(label, topic)                 \
    {                                       \
        if (run_http_tests)                 \
        {                                   \
            SECTION("HTTP")                 \
            {                               \
                set_http_connection();      \
                test_method();              \
            }                               \
        }                                   \
        if (run_ws_tests)                   \
        {                                   \
            SECTION("WS")                   \
            {                               \
                set_ws_connection();        \
                test_method();              \
            }                               \
        }                                   \
        if (run_wss_tests)                  \
        {                                   \
            SECTION("WSS")                  \
            {                               \
                set_wss_connection();       \
                test_method();              \
            }                               \
        }                                   \
    }

// ----- Helper for validation of the expected keys in a json map -------

template <typename... Keys> inline auto contains_keys(nlohmann::json const& map, std::string key, Keys&&... keys)
{
    if constexpr (0 == sizeof...(Keys))
    {
        return map.contains(key);
    }
    else
    {
        return map.contains(key) && contains_keys(map, std::forward<Keys>(keys)...);
    }
}
