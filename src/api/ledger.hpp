#include "sdk.hpp"

#include <cstdint>

namespace ledger
{
    /// @brief This API call will return the latest momentum.
    /// @return the latest momentum.
    auto get_frontier_momentum() -> sdk::return_t<sdk::json_t>;
} // namespace ledger
