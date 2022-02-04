#include "connection_error.hpp"
#include "connection_http.hpp"
#include "connection_ws.hpp"
#include "connection_wss.hpp"
#include "sdk.hpp"

#include <catch2/catch_test_macros.hpp>

// ----- Definition of connection variables -----------------------------

constexpr auto const http_host = "";
constexpr auto const ws_host = "";
constexpr auto const wss_host = "";
constexpr uint16_t const http_port = 35997;
constexpr uint16_t const ws_port = 35998;
constexpr uint16_t const wss_port = 443;

// ----- Helper functions -----------------------------------------------

auto set_http_connection() { sdk::set_connector<sdk::http_connector>(http_host, http_port); }
auto set_ws_connection() { sdk::set_connector<sdk::ws_connector>(ws_host, ws_port); }
auto set_wss_connection() { sdk::set_connector<sdk::wss_connector>(wss_host, wss_port); }

TEST_CASE("Uninitialized reports unconnected", "[HTTP]") { REQUIRE_FALSE(sdk::connected()); }
TEST_CASE("Uninitialized reports unconnected", "[WS]") { REQUIRE_FALSE(sdk::connected()); }
TEST_CASE("Uninitialized reports unconnected", "[WSS]") { REQUIRE_FALSE(sdk::connected()); }

TEST_CASE("Can connect to known unsecure host", "[HTTP]")
{
    using namespace std::chrono_literals;

    set_http_connection();
    std::this_thread::sleep_for(1s);
    REQUIRE(sdk::connected());
}

TEST_CASE("Can connect to known unsecure host", "[WS]")
{
    using namespace std::chrono_literals;

    set_ws_connection();
    std::this_thread::sleep_for(1s);
    REQUIRE(sdk::connected());
}

TEST_CASE("Can connect to known secure host", "[WSS]")
{
    using namespace std::chrono_literals;

    set_wss_connection();
    std::this_thread::sleep_for(1s);
    REQUIRE(sdk::connected());
}

TEST_CASE("Invalid host connection throws and remains unconnected", "[HTTP]")
{
    REQUIRE_THROWS_AS(sdk::set_connector<sdk::http_connector>("nonexisting", 8000), sdk::connection_error);
    REQUIRE_FALSE(sdk::connected());
}

TEST_CASE("Invalid host connection throws and remains unconnected", "[WS]")
{
    REQUIRE_THROWS_AS(sdk::set_connector<sdk::ws_connector>("nonexisting", 8000), sdk::connection_error);
    REQUIRE_FALSE(sdk::connected());
}

TEST_CASE("Invalid host connection throws and remains unconnected", "[WSS]")
{
    REQUIRE_THROWS_AS(sdk::set_connector<sdk::wss_connector>("nonexisting", 8000), sdk::connection_error);
    REQUIRE_FALSE(sdk::connected());
}
