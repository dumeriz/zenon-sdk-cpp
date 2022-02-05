#include "connection_error.hpp"
#include "connection_http.hpp"
#include "connection_ws.hpp"
#include "connection_wss.hpp"
#include "helper.hpp"
#include "sdk.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Uninitialized reports unconnected", "[HTTP]") { REQUIRE_FALSE(sdk::connected()); }
TEST_CASE("Uninitialized reports unconnected", "[WS]") { REQUIRE_FALSE(sdk::connected()); }
TEST_CASE("Uninitialized reports unconnected", "[WSS]") { REQUIRE_FALSE(sdk::connected()); }

auto test_connection()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    REQUIRE(sdk::connected());
}

DEF_TEST("Can connect", "[Connection]", test_connection);

TEST_CASE("Invalid host connection throws and remains unconnected", "[Connection]")
{
    if (run_http_tests)
    {
        SECTION("HTTP")
        {
            REQUIRE_THROWS_AS(sdk::set_connector<sdk::http_connector>("nonexisting", 0), sdk::connection_error);
            REQUIRE_FALSE(sdk::connected());
        }
    }
    if (run_ws_tests)
    {
        SECTION("WS")
        {
            REQUIRE_THROWS_AS(sdk::set_connector<sdk::ws_connector>("nonexisting", 0), sdk::connection_error);
            REQUIRE_FALSE(sdk::connected());
        }
    }
    if (run_wss_tests)
    {
        SECTION("WSS")
        {
            REQUIRE_THROWS_AS(sdk::set_connector<sdk::wss_connector>("nonexisting", 0), sdk::connection_error);
            REQUIRE_FALSE(sdk::connected());
        }
    }
}
