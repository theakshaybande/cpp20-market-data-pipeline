#pragma once
#include "md_event.hpp"
#include <unordered_map>
#include <cstdint>

struct SymbolStats {
    int64_t trades = 0;
    int64_t buy_qty = 0;
    int64_t sell_qty = 0;
};

class Stats {
public:
    void on_event(const MdEvent& ev) {
        auto& s = stats_[ev.symbol];
        s.trades++;
        if (ev.side == Side::Buy) s.buy_qty += ev.qty;
        else s.sell_qty += ev.qty;
    }

    const auto& data() const { return stats_; }

private:
    std::unordered_map<std::string_view, SymbolStats> stats_;
};
