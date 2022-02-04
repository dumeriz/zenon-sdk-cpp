#pragma once

#include "connection_error.hpp"
#include "connection_http.hpp"
#include "connection_ws.hpp"
#include "connection_wss.hpp"
#include "sdk.hpp"

#include <nlohmann/json.hpp>
#include <catch2/catch_test_macros.hpp>

// ----- Definition of connection variables -----------------------------

constexpr auto const http_host = "";
constexpr auto const ws_host = "";
constexpr auto const wss_host = "";
constexpr uint16_t const http_port = 35997;
constexpr uint16_t const ws_port = 35998;
constexpr uint16_t const wss_port = 443;

// ----- Helper functions -----------------------------------------------

auto inline set_http_connection() { sdk::set_connector<sdk::http_connector>(http_host, http_port); }
auto inline set_ws_connection() { sdk::set_connector<sdk::ws_connector>(ws_host, ws_port); }
auto inline set_wss_connection() { sdk::set_connector<sdk::wss_connector>(wss_host, wss_port); }
//auto inline set_wss_connection() { sdk::set_connector<sdk::eidheim_connector>(wss_host, wss_port); }

// ----- Macro to simplify test executions for both http and ws ---------

#define WSS_TEST(label, topic, test_method) \
TEST_CASE(label, topic) \
{ \
 set_wss_connection(); \
 test_method(); \
}

#define DEF_TEST(label, topic, test_method) \
    TEST_CASE(label, topic)                 \
    {                                       \
        SECTION("HTTP")                     \
        {                                   \
            try                             \
            {                               \
                set_http_connection();      \
                test_method();              \
            }                               \
            catch (...)                     \
            {                               \
            }                               \
        }                                   \
        SECTION("WS")                       \
        {                                   \
            try                             \
            {                               \
                set_ws_connection();        \
                test_method();              \
            }                               \
            catch (...)                     \
            {                               \
            }                               \
        }                                   \
        SECTION("WSS")                       \
        {                                   \
            try                             \
            {                               \
                set_wss_connection();        \
                test_method();              \
            }                               \
            catch (...)                     \
            {                               \
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

