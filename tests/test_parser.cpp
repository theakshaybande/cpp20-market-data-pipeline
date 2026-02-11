#include "parser.hpp"

#include <cassert>

int main() {
    mdp::Parser parser;

    auto trade = parser.parse_line("T,FOO,123,10.5,100,0,0");
    assert(trade);
    assert(trade.value().type == mdp::EventType::Trade);
    assert(trade.value().trade.size == 100);

    auto quote = parser.parse_line("Q,FOO,123,10.4,10.6,50,60");
    assert(quote);
    assert(quote.value().type == mdp::EventType::Quote);
    assert(quote.value().quote.bid_size == 50);

    auto bad = parser.parse_line("X,FOO,1,2,3,4,5");
    assert(!bad);

    return 0;
}
