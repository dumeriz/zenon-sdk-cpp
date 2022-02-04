#include "helper.hpp"
#include "api_embedded.hpp"

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
    auto str(actual.dump());

    REQUIRE(contains_keys(actual, "count", "list"));

    for (size_t i{}; i < amount; i++)
    {
        REQUIRE(actual["list"][i].size() == 13);
        REQUIRE(contains_keys(actual["list"][i], "currentStats", "giveDelegateRewardPercentage",
                              "giveMomentumRewardPercentage", "isRevocable", "name", "ownerAddress", "producerAddress",
                              "rank", "revokeCooldown", "revokeTimestamp", "type", "weight", "withdrawAddress"));

        REQUIRE(actual["list"][i]["currentStats"].size() == 2);
        REQUIRE(contains_keys(actual["list"][i]["currentStats"], "expectedMomentums", "producedMomentums"));
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

auto pillar_get_by_owner() { REQUIRE(false); }

auto pillar_get_by_name() { REQUIRE(false); }

auto pillar_get_delegated_pillar() { REQUIRE(false); }

auto pillar_get_deposited_qsr() { REQUIRE(false); }

auto pillar_get_uncollected_reward() { REQUIRE(false); }

auto pillar_get_frontier_reward_per_page() { REQUIRE(false); }

// ----- Wrapping of test definitions in catch2-test macros -------------
// Every test method from above must have an entry here.

WSS_TEST("Pillar registration cost looks correct", "[embedded.pillar]", pillar_get_qsr_registration_cost);
//WSS_TEST("List of pillars has expected entries", "[embedded.pillar]", pillar_get_all);
/*
DEF_TEST("Pillar name availability-check succeeds", "[embedded.pillar]", pillar_check_name_availability);
DEF_TEST("Pillar can be retrieveed by owner", "[embedded.pillar]", pillar_get_by_owner);
DEF_TEST("Pillar can be retrieveed by name", "[embedded.pillar]", pillar_get_by_name);
DEF_TEST("Pillar delegations can be retrieved", "[embedded.pillar]", pillar_get_delegated_pillar);
DEF_TEST("Deposited qsr matches pillar cost or 0 for random", "[embedded.pillar]", pillar_get_deposited_qsr);
DEF_TEST("Uncollected pillar rewards are reported", "[embedded.pillar]", pillar_get_uncollected_reward);
DEF_TEST("Frontier rewards match expected format", "[embedded.pillar]", pillar_get_frontier_reward_per_page);
*/
