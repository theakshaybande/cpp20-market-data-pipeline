#pragma once
#include <atomic>
#include <array>
#include <cstddef>
#include <optional>

template <typename T, std::size_t N>
class SpscRingQueue {
    static_assert((N & (N - 1)) == 0, "N must be power of 2");

public:
    bool push(const T& v) {
        auto head = head_.load(std::memory_order_relaxed);
        auto next = (head + 1) & (N - 1);
        if (next == tail_.load(std::memory_order_acquire)) return false;
        buf_[head] = v;
        head_.store(next, std::memory_order_release);
        return true;
    }

    std::optional<T> pop() {
        auto tail = tail_.load(std::memory_order_relaxed);
        if (tail == head_.load(std::memory_order_acquire)) return std::nullopt;
        T v = buf_[tail];
        tail_.store((tail + 1) & (N - 1), std::memory_order_release);
        return v;
    }

private:
    alignas(64) std::atomic<std::size_t> head_{0};
    alignas(64) std::atomic<std::size_t> tail_{0};
    std::array<T, N> buf_{};
};
