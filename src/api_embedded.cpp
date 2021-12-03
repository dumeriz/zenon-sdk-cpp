#include "api_embedded.hpp"

#include "internal.hpp"

extern sdk::detail::api_connection connection;

auto
embedded::pillar::get_qsr_registration_cost() -> uint64_t
{
    return connection.call<uint64_t>(1, "embedded.pillar.getQsrRegistrationCost");
}
