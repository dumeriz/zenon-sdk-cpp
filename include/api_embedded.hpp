#include "nlohmann/json.hpp"

#include <cstdint>

namespace embedded::pillar
{
    /// @brief Returns the current amount of QSR required to setup a pillar.
    /// @return the amount required.
    auto get_qsr_registration_cost() -> uint64_t;

    /// @brief Returns a paged list of existing pillars.
    /// @param page is the requested page
    /// @param count is the number of entries to be displayed on that page.
    /// @return an array of pillar information objects.
    auto get_all(size_t page, size_t count) -> nlohmann::json;

    /// @brief Returns whether the specified name is not yet used as a pillar identifier.
    /// @param name is the name to be checked.
    /// @return true if the name is not used currently.
    auto check_name_availability(std::string_view name) -> bool;
} // namespace embedded::pillar
