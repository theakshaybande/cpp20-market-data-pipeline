#include "parser.hpp"

#include <charconv>
#include <cstdlib>
#include <string>
#include <string_view>

namespace mdp {

namespace {

bool split_fields(std::string_view line, std::string_view out[7]) {
    std::size_t pos = 0;
    for (int i = 0; i < 7; ++i) {
        const std::size_t next = line.find(',', pos);
        if (next == std::string_view::npos) {
            if (i != 6) {
                return false;
            }
            out[i] = line.substr(pos);
            return true;
        }
        out[i] = line.substr(pos, next - pos);
        pos = next + 1;
    }
    return true;
}

template <typename T>
bool parse_int(std::string_view field, T& out) {
    const char* begin = field.data();
    const char* end = field.data() + field.size();
    auto res = std::from_chars(begin, end, out);
    return res.ec == std::errc() && res.ptr == end;
}

bool parse_double(std::string_view field, double& out) {
    char* end = nullptr;
    std::string tmp(field);
    out = std::strtod(tmp.c_str(), &end);
    return end && *end == '\0';
}

} // namespace

Expected<Event, ParseError> Parser::parse_line(std::string_view line) const {
    std::string_view fields[7] = {};
    if (!split_fields(line, fields)) {
        return ParseError::InvalidFormat;
    }

    if (fields[0].size() != 1) {
        return ParseError::UnknownType;
    }

    const char type_char = fields[0][0];
    Event ev{};
    ev.symbol = fields[1];

    if (type_char == 'T') {
        ev.type = EventType::Trade;
        if (!parse_int(fields[2], ev.trade.ts)) return ParseError::InvalidFormat;
        if (!parse_double(fields[3], ev.trade.price)) return ParseError::InvalidFormat;
        if (!parse_int(fields[4], ev.trade.size)) return ParseError::InvalidFormat;
        return ev;
    }

    if (type_char == 'Q') {
        ev.type = EventType::Quote;
        if (!parse_int(fields[2], ev.quote.ts)) return ParseError::InvalidFormat;
        if (!parse_double(fields[3], ev.quote.bid)) return ParseError::InvalidFormat;
        if (!parse_double(fields[4], ev.quote.ask)) return ParseError::InvalidFormat;
        if (!parse_int(fields[5], ev.quote.bid_size)) return ParseError::InvalidFormat;
        if (!parse_int(fields[6], ev.quote.ask_size)) return ParseError::InvalidFormat;
        return ev;
    }

    return ParseError::UnknownType;
}

} // namespace mdp
