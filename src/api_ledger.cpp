#include "api_ledger.hpp"

#include "internal.hpp"

extern sdk::detail::api_connection connection;

auto ledger::get_frontier_momentum() -> nlohmann::json
{
    return connection.call<nlohmann::json>(31, "ledger.getFrontierMomentum");
}
