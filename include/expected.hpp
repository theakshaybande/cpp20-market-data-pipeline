#pragma once
#include <variant>
#include <string>
#include <utility>

template <typename T>
class Expected {
public:
    static Expected ok(T v) { return Expected(std::move(v)); }
    static Expected err(std::string e) { return Expected(std::move(e)); }

    [[nodiscard]] bool has_value() const { return std::holds_alternative<T>(data_); }
    [[nodiscard]] const T& value() const { return std::get<T>(data_); }
    [[nodiscard]] T& value() { return std::get<T>(data_); }
    [[nodiscard]] const std::string& error() const { return std::get<std::string>(data_); }

private:
    std::variant<T, std::string> data_;

    explicit Expected(T v) : data_(std::move(v)) {}
    explicit Expected(std::string e) : data_(std::move(e)) {}
};
