#include "api_embedded.hpp"
#include "connection_http.hpp"
#include "connection_ws.hpp"
#include "sdk.hpp"

#include <catch2/catch_test_macros.hpp>

template <typename C>
auto
pillar_registration_cost_test(std::string host, uint16_t port)
{
    sdk::set_connector(std::make_shared<C>(host, port));

    auto actual = embedded::pillar::get_qsr_registration_cost();

    auto expected_least = uint64_t{23000000000000}; // cost as of Nov. 30 2021
    auto inc_per_pillar = uint64_t{1000000000000};  // each pillar raises the total cost

    REQUIRE(expected_least <= actual);
    REQUIRE(0 == (expected_least % inc_per_pillar));
}

TEST_CASE("Pillar registration cost looks correct", "[embedded.pillar]")
{
    pillar_registration_cost_test<sdk::http_connector>("0.0.0.0", 35997);
    pillar_registration_cost_test<sdk::ws_connector>("ws://localhost", 35998);
}
