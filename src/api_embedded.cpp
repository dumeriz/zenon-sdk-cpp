#include "api_embedded.hpp"

#include "internal.hpp"

extern sdk::detail::api_connection connection;

auto embedded::pillar::get_qsr_registration_cost() -> uint64_t
{
    return connection.call<uint64_t>(1, "embedded.pillar.getQsrRegistrationCost");
}

auto embedded::pillar::get_all(size_t page, size_t amount) -> nlohmann::json
{
    return connection.call<nlohmann::json>(3, "embedded.pillar.getAll", page, amount);
}

auto embedded::pillar::check_name_availability(std::string_view name) -> bool
{
    return connection.call<bool>(9, "embedded.pillar.checkNameAvailability", name);
}
