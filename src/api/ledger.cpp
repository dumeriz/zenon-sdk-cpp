#include "api/ledger.hpp"
#include "internal.hpp"

extern sdk::detail::api_connection connection;

auto ledger::get_frontier_momentum() -> sdk::return_t<sdk::json_t>
{
    return connection.call<nlohmann::json>(31, "ledger.getFrontierMomentum");
}
