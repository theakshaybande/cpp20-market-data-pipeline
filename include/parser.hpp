#pragma once
#include "expected.hpp"
#include "md_event.hpp"
#include <string_view>
#include <charconv>
#include <array>

namespace detail {

inline Expected<int64_t> parse_i64(std::string_view s) {
    int64_t v{};
    auto* b = s.data();
    auto* e = s.data() + s.size();
    auto [ptr, ec] = std::from_chars(b, e, v);
    if (ec != std::errc{} || ptr != e) return Expected<int64_t>::err("bad int64");
    return Expected<int64_t>::ok(v);
}

inline Expected<int32_t> parse_i32(std::string_view s) {
    int32_t v{};
    auto* b = s.data();
    auto* e = s.data() + s.size();
    auto [ptr, ec] = std::from_chars(b, e, v);
    if (ec != std::errc{} || ptr != e) return Expected<int32_t>::err("bad int32");
    return Expected<int32_t>::ok(v);
}

inline Expected<Side> parse_side(std::string_view s) {
    if (s == "B") return Expected<Side>::ok(Side::Buy);
    if (s == "S") return Expected<Side>::ok(Side::Sell);
    return Expected<Side>::err("bad side");
}

inline std::array<std::string_view, 5> split5(std::string_view line) {
    std::array<std::string_view, 5> out{};
    size_t start = 0;
    int idx = 0;

    for (size_t i = 0; i <= line.size() && idx < 5; ++i) {
        if (i == line.size() || line[i] == ',') {
            out[idx++] = line.substr(start, i - start);
            start = i + 1;
        }
    }
    return out;
}

} // namespace detail

inline Expected<MdEvent> parse_md_event(std::string_view line) {
    auto parts = detail::split5(line);

    auto ts  = detail::parse_i64(parts[0]);
    auto sd  = detail::parse_side(parts[2]);
    auto px  = detail::parse_i64(parts[3]);
    auto qty = detail::parse_i32(parts[4]);

    if (!ts.has_value())  return Expected<MdEvent>::err("timestamp: " + ts.error());
    if (!sd.has_value())  return Expected<MdEvent>::err("side: " + sd.error());
    if (!px.has_value())  return Expected<MdEvent>::err("price: " + px.error());
    if (!qty.has_value()) return Expected<MdEvent>::err("qty: " + qty.error());

    MdEvent ev;
    ev.ts = std::chrono::nanoseconds{ts.value()};
    ev.symbol = parts[1];
    ev.side = sd.value();
    ev.price_ticks = px.value();
    ev.qty = qty.value();

    return Expected<MdEvent>::ok(ev);
}
