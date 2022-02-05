#include "nlohmann/json.hpp"

#include <cstdint>

namespace ledger
{
    /// @brief This API call will return the latest momentum.
    /// @return the latest momentum.
    auto get_frontier_momentum() -> nlohmann::json;
} // namespace ledger
