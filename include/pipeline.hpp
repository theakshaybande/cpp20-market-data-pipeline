#pragma once
#include "parser.hpp"
#include "ring_queue.hpp"
#include "stats.hpp"
#include <string_view>

template <std::size_t QSIZE>
class Pipeline {
public:
    using Queue = SpscRingQueue<MdEvent, QSIZE>;

    Expected<void*> ingest_line(std::string_view line) {
        auto ev = parse_md_event(line);
        if (!ev.has_value()) return Expected<void*>::err(ev.error());
        if (!q_.push(ev.value())) return Expected<void*>::err("queue full");
        return Expected<void*>::ok(nullptr);
    }

    void drain() {
        while (auto ev = q_.pop()) {
            stats_.on_event(*ev);
        }
    }

    const Stats& stats() const { return stats_; }

private:
    Queue q_{};
    Stats stats_{};
};
