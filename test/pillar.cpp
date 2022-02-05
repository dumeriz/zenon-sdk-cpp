#include "api/embedded/pillar.hpp"
#include "helper.hpp"
#include "sdk.hpp"

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

    REQUIRE(actual);
    REQUIRE(expected_least <= actual.value);
    REQUIRE(0 == ((actual.value - expected_least) % inc_per_pillar));
}

auto validate_pillar_information(sdk::json_t const& js)
{
    REQUIRE(js.size() == 13);
    REQUIRE(contains_keys(js, "currentStats", "giveDelegateRewardPercentage", "giveMomentumRewardPercentage",
                          "isRevocable", "name", "ownerAddress", "producerAddress", "rank", "revokeCooldown",
                          "revokeTimestamp", "type", "weight", "withdrawAddress"));

    REQUIRE(js["currentStats"].size() == 2);
    REQUIRE(contains_keys(js["currentStats"], "expectedMomentums", "producedMomentums"));
}

auto pillar_get_all()
{
    size_t amount = 5;
    auto actual = embedded::pillar::get_all(0, amount);
    REQUIRE(actual);
    //std::cout << actual.value.dump() << std::endl;

    REQUIRE(contains_keys(actual.value, "count", "list"));
    REQUIRE(actual.value["list"].size() == amount);

    for (size_t i{}; i < amount; i++)
    {
        validate_pillar_information(actual.value["list"][i]);
    }
}

auto pillar_check_name_availability()
{
    std::string exists{embedded::pillar::get_all(0, 1).value["list"][0]["name"]};
    auto does_not_exist = "reallyNotALikelyName";

    auto not_available{embedded::pillar::check_name_availability(exists)};
    REQUIRE(not_available);
    REQUIRE(!not_available.value);

    auto available{embedded::pillar::check_name_availability(does_not_exist)};
    REQUIRE(available);
    REQUIRE(available.value);
}

auto pillar_get_by_owner()
{
    std::string const exists{embedded::pillar::get_all(0, 1).value["list"][0]["ownerAddress"]};
    auto does_not_exist{exists};
    does_not_exist.replace(0, 1, "X");

    auto existing = embedded::pillar::get_by_owner(exists);
    auto non_existing = embedded::pillar::get_by_owner(does_not_exist);
    //std::cout << existing.value.dump() << std::endl;

    REQUIRE(existing);
    REQUIRE_FALSE(non_existing);
    for (size_t i{}; i < existing.value.size(); i++)
    {
        validate_pillar_information(existing.value[i]);
    }
}

auto pillar_get_by_name()
{ 
    std::string const exists{embedded::pillar::get_all(0, 1).value["list"][0]["name"]};
    std::string does_not_exist{"unlikely name"};

    auto existing = embedded::pillar::get_by_name(exists);
    auto non_existing = embedded::pillar::get_by_name(does_not_exist);
    //std::cout << existing.value.dump() << std::endl;

    REQUIRE(existing);
    REQUIRE(non_existing);
    validate_pillar_information(existing.value);
}

auto pillar_get_delegated_pillar()
{
    std::string const exists{embedded::pillar::get_all(0, 1).value["list"][0]["ownerAddress"]};
    std::string does_not_exist{exists};
    does_not_exist.replace(0, 1, "X");

    auto existing = embedded::pillar::get_delegated_pillar(exists);
    auto non_existing = embedded::pillar::get_delegated_pillar(does_not_exist);
    //std::cout << existing.value.dump() << std::endl;

    // null?
    REQUIRE(existing);
    REQUIRE_FALSE(non_existing);
    REQUIRE(existing.value.size() == 3);
    REQUIRE(contains_keys(existing.value, "name", "status", "weight"));
}

auto pillar_get_deposited_qsr()
{
    std::string const exists{embedded::pillar::get_all(0, 1).value["list"][0]["ownerAddress"]};

    auto existing = embedded::pillar::get_deposited_qsr(exists);

    //std::cout << existing.value << std::endl;
    REQUIRE(existing);
    REQUIRE(existing.value == 0); // active pillars have burned their qsr
}

auto pillar_get_uncollected_reward()
{
    std::string const exists{embedded::pillar::get_all(0, 1).value["list"][0]["ownerAddress"]};
    auto actual = embedded::pillar::get_uncollected_reward(exists);
    REQUIRE(actual);

        REQUIRE(3 == actual.value.size());
        REQUIRE(contains_keys(actual.value, "address", "znnAmount", "qsrAmount"));
        REQUIRE(0 == actual.value["qsrAmount"]);
}

auto pillar_get_frontier_reward_per_page()
{
    size_t amount = 5;
    std::string const exists{embedded::pillar::get_all(0, 1).value["list"][0]["ownerAddress"]};
    auto actual = embedded::pillar::get_frontier_reward_by_page(exists, 0, 5);
    REQUIRE(actual);

    REQUIRE(contains_keys(actual.value, "count", "list"));
    REQUIRE(actual.value["list"].size() == amount);
    REQUIRE(actual.value["count"] >= actual.value["list"].size());

    for (size_t i{}; i < amount; i++)
    {
        REQUIRE(3 == actual.value["list"][i].size());
        REQUIRE(contains_keys(actual.value["list"][i], "epoch", "znnAmount", "qsrAmount"));
        REQUIRE(0 == actual.value["list"][i]["qsrAmount"]);
    }
}

// ----- Wrapping of test definitions in catch2-test macros -------------
// Every test method from above must have an entry here.

DEF_TEST("Pillar registration cost looks correct", "[embedded.pillar]", pillar_get_qsr_registration_cost);
DEF_TEST("List of pillars has expected entries", "[embedded.pillar]", pillar_get_all);
DEF_TEST("Pillar name availability-check succeeds", "[embedded.pillar]", pillar_check_name_availability);
DEF_TEST("Pillar can be retrieveed by owner", "[embedded.pillar]", pillar_get_by_owner);
DEF_TEST("Pillar can be retrieveed by name", "[embedded.pillar]", pillar_get_by_name);
// TODO: doesn't work currently; api returns null. Why?
//DEF_TEST("Pillar delegations can be retrieved", "[embedded.pillar]", pillar_get_delegated_pillar);
DEF_TEST("Deposited qsr matches pillar cost or 0 for random", "[embedded.pillar]", pillar_get_deposited_qsr);
DEF_TEST("Uncollected pillar rewards are reported", "[embedded.pillar]", pillar_get_uncollected_reward);
DEF_TEST("Frontier rewards match expected format", "[embedded.pillar]", pillar_get_frontier_reward_per_page);
