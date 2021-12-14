#include "api_embedded.hpp"
#include "connection_http.hpp"
#include "connection_ws.hpp"
#include "sdk.hpp"

#include <catch2/catch_test_macros.hpp>


// ----- Definition of connection variables -----------------------------

constexpr auto const http_host = "0.0.0.0";
constexpr auto const ws_host = "ws://localhost";
constexpr uint16_t const http_port = 35997;
constexpr uint16_t const ws_port = 35998;

// ----- Helper functions -----------------------------------------------

template<typename C> auto set_connector(std::string host, uint16_t port)
{
    sdk::set_connector(std::make_shared<C>(host, port));
}

auto set_http_connection()
{
    set_connector<sdk::http_connector>(http_host, http_port);
}

auto set_ws_connection()
{
    set_connector<sdk::ws_connector>(ws_host, ws_port);
}

// ----- Macro to simplify test executions for both http and ws ---------

#define DEF_TEST(label, topic, test_method) \
    TEST_CASE(label, topic) { \
        set_http_connection(); test_method(); \
        set_ws_connection(); test_method(); \
    }


// ----- Helper for validation of the expected keys in a json map -------

template<typename ... Keys>
auto contains_keys(nlohmann::json const& map, std::string key, Keys&& ... keys)
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


// ----- Test definitions -----------------------------------------------

// To add a test, first implement a dummy for the required interface in
// include/api_embedded.cpp and src/api_embedded.cpp
// Then define one or multiple test methods, calling the dummy method.
// When that compiles correctly, implement the correct interface and
// validate that the tests succeed.
// Give the test methods names that reflect the exercised methods name.
// Don't forget to add the method to the test set by calling the DEF_TEST
// macro at the bottom of this file. This takes care that the test method
// will be called via both http and ws connections.

auto pillar_get_qsr_registration_cost()
{
    auto actual = embedded::pillar::get_qsr_registration_cost();

    auto expected_least = uint64_t{23000000000000}; // cost as of Nov. 30 2021
    auto inc_per_pillar = uint64_t{1000000000000};  // each pillar raises the total cost

    REQUIRE(expected_least <= actual);
    REQUIRE(0 == (expected_least % inc_per_pillar));
}

auto pillar_get_all()
{
    size_t amount = 5;
    auto actual = embedded::pillar::get_all(0, amount);


    REQUIRE(contains_keys(actual, "count", "list"));

    for (size_t i{}; i < amount; i++)
    {
        REQUIRE(actual["list"][i].size() == 13);
        REQUIRE(contains_keys(actual["list"][i],
                    "currentStats", "giveDelegateRewardPercentage", "giveMomentumRewardPercentage",
                    "isRevocable", "name", "ownerAddress", "producerAddress", "rank", "revokeCooldown", "revokeTimestamp",
                    "type", "weight", "withdrawAddress"));

        REQUIRE(actual["list"][i]["currentStats"].size() == 2);
        REQUIRE(contains_keys(actual["list"][i]["currentStats"],
                    "expectedMomentums", "producedMomentums"));
    }

    // we could also check the types returned. checking the values is not useful as these change over time.
    // std::cout << actual << std::endl;
}

auto pillar_check_name_availability()
{
    std::string exists = embedded::pillar::get_all(0, 1)["list"][0]["name"];
    auto does_not_exist = "reallyNotALikelyName";

    REQUIRE(!embedded::pillar::check_name_availability(exists));
    REQUIRE(embedded::pillar::check_name_availability(does_not_exist));
}

// ----- Wrapping of test definitions in catch2-test macros -------------
// Every test method from above must have an entry here.

DEF_TEST("Pillar registration cost looks correct", "[embedded.pillar]", pillar_get_qsr_registration_cost);
DEF_TEST("List of pillars has expected entries", "[embedded.pillar]", pillar_get_all);
DEF_TEST("Pillar name availability-check succeeds", "[embedded.pillar]", pillar_check_name_availability);
