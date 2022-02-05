#include "api/embedded/pillar.hpp"
#include "internal.hpp"
#include "sdk.hpp"

#include <algorithm>
#include <iterator>

extern sdk::detail::api_connection connection;

/// @brief Return a copy of the parameter converted to all lowercase.
/// The api expects address arguments to be given in either lower or uppercase; mixed case is an error.
auto normalize_address(std::string_view address) -> std::string
{
    std::string normalized;
    std::transform(address.begin(), address.end(), std::back_inserter(normalized),
                   [](auto const& c) { return std::tolower(c); });

    return normalized;
}

auto embedded::pillar::get_qsr_registration_cost() -> sdk::return_t<uint64_t>
{
    return connection.call<uint64_t>(2, "embedded.pillar.getQsrRegistrationCost");
}

auto embedded::pillar::check_name_availability(std::string_view name) -> sdk::return_t<bool>
{
    return connection.call<bool>(9, "embedded.pillar.checkNameAvailability", name);
}

auto embedded::pillar::get_all(size_t page, size_t amount) -> sdk::return_t<sdk::json_t>
{
    return connection.call<sdk::json_t>(3, "embedded.pillar.getAll", page, amount);
}

auto embedded::pillar::get_by_owner(std::string_view owner_address) -> sdk::return_t<sdk::json_t>
{
    return connection.call<sdk::json_t>(4, "embedded.pillar.getByOwner", normalize_address(owner_address));
}

auto embedded::pillar::get_by_name(std::string_view pillar_name) -> sdk::return_t<sdk::json_t>
{
    return connection.call<sdk::json_t>(4, "embedded.pillar.getByName", pillar_name);
}

auto embedded::pillar::get_delegated_pillar(std::string_view owner_address) -> sdk::return_t<sdk::json_t>
{
    return connection.call<sdk::json_t>(4, "embedded.pillar.getDelegatedPillar", owner_address);
}

auto embedded::pillar::get_deposited_qsr(std::string_view address) -> sdk::return_t<uint64_t>
{
    return connection.call<uint64_t>(4, "embedded.pillar.getDepositedQsr", address);
}

auto embedded::pillar::get_uncollected_reward(std::string_view owner_address) -> sdk::return_t<sdk::json_t>
{
    return connection.call<sdk::json_t>(4, "embedded.pillar.getUncollectedReward", owner_address);
}

auto embedded::pillar::get_frontier_reward_by_page(std::string_view address, size_t page, size_t count)
    -> sdk::return_t<sdk::json_t>
{
    return connection.call<sdk::json_t>(4, "embedded.pillar.getFrontierRewardByPage", address, page, count);
}
