#include "sdk.hpp"

#include <cstdint>

namespace embedded::pillar
{
    /// @brief Returns the current amount of QSR required to setup a pillar.
    /// @return the amount required.
    auto get_qsr_registration_cost() -> sdk::return_t<uint64_t>;

    /// @brief Returns information about the availability of a name for a Pillar.
    /// @param name is the name to be checked.
    /// @return true if the name is not used currently.
    auto check_name_availability(std::string_view name) -> sdk::return_t<bool>;

    /// @brief Returns the list of Pillars in the network with additional information.
    /// @param page is the requested page
    /// @param count is the number of entries to be displayed on that page.
    /// @return an array of pillar information objects.
    auto get_all(size_t page, size_t count) -> sdk::return_t<sdk::json_t>;

    /// @brief Returns all the Pillars registered by an address.
    /// @param owner_address represents the owner.
    /// @return an array of pillar entries.
    auto get_by_owner(std::string_view owner_address) -> sdk::return_t<sdk::json_t>;

    /// @brief Returns information about the Pillar with the specified name.
    /// @param pillar_name is the name of the pillar.
    /// @return an object containing information about the pillar.
    auto get_by_name(std::string_view pillar_name) -> sdk::return_t<sdk::json_t>;

    /// @brief Returns the total number of delegations for a particular Pillar.
    /// @param owner_address represents the owner.
    /// @return an object containing the keys name (string), status (number) and weight (number).
    auto get_delegated_pillar(std::string_view owner_address) -> sdk::return_t<sdk::json_t>;

    /// @brief Return the amount of QSR deposited that can be used to create a Pillar.
    /// @param address is the address to query.
    /// @return the amount of deposited qsr.
    auto get_deposited_qsr(std::string_view address) -> sdk::return_t<uint64_t>;

    /// @brief Return the uncollected reward for the specified pillar.
    /// @param owner_address represents the owner.
    /// @return an object with keys address (string), znnAmount (number), qsrAmount (number).
    auto get_uncollected_reward(std::string_view owner_address) -> sdk::return_t<sdk::json_t>;

    /// @brief Return reward information about the specified pillar for a specified range of pages.
    /// @param address represents the pillar address.
    /// @param page represents the page index.
    /// @param count represents the number of entries to be displayed on this page.
    /// @return an object with key count (string) and an array of count elements,
    ///         each containing keys epoch (number), znnAmount (number) and qsrAmount (number).
    auto get_frontier_reward_by_page(std::string_view address, size_t page, size_t count) -> sdk::return_t<sdk::json_t>;

} // namespace embedded::pillar
