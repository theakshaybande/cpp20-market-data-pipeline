#include "pipeline.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <format>

int main() {
    Pipeline<1024> pipe;

    // In real system, you'd read from file/socket into one large buffer and take string_view slices.
    std::vector<std::string> lines = {
        "1700000000000000000,EURUSD,B,100234,5",
        "1700000000000000100,EURUSD,S,100233,3",
        "1700000000000000200,USDJPY,B,150001,2",
        "1700000000000000300,EURUSD,B,100235,1"
    };

    for (auto& l : lines) {
        auto r = pipe.ingest_line(l);
        if (!r.has_value()) {
            std::cerr << "Parse/ingest error: " << r.error() << "\n";
        }
    }

    pipe.drain();

    for (const auto& [sym, st] : pipe.stats().data()) {
        std::cout << std::format(
            "{} trades={} buy_qty={} sell_qty={}\n",
            sym, st.trades, st.buy_qty, st.sell_qty
        );
    }
}
