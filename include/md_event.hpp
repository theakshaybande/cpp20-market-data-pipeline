#pragma once
#include "types.hpp"
#include <string_view>
#include <chrono>

struct MdEvent {
    std::string_view symbol;  // zero-copy view into input buffer
    Price price_ticks{};
    Quantity qty{};
    Side side{};
    std::chrono::nanoseconds ts{};
};
