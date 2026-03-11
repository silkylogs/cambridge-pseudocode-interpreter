#include "cpi.hpp"

Integer::Integer(std::string sv) : data_{ std::stoi(sv) } {
}

std::string Integer::to_string() {
    return std::to_string(data_);
}

Real::Real(std::string sv) : data_{ std::stof(sv) } {
}

std::string Real::to_string() {
    return std::to_string(data_);
}

Char::Char(std::string sv) : data_{ sv[0] } {
}

std::string Char::to_string() {
    std::string s = "";
    s.push_back(data_);
    return s;
}

String::String(std::string sv) : data_{ sv } {
}

std::string String::to_string() {
    return data_;
}

Boolean::Boolean(std::string sv) : data_{} {
    lower(sv);
    if (sv == "true") data_ = true;
    else if (sv == "false") data_ = false;
    else throw std::invalid_argument("Cannot parse as bool");
}

std::string Boolean::to_string() {
    return std::to_string(data_);
}

Date::Date(std::string sv) : d_{}, m_{}, y_{} {
    auto first = sv.find('/');
    auto second = sv.find('/', first + 1);

    std::string date = sv.substr(0, first);
    d_ = std::stoi(date);

    std::string month = sv.substr(first + 1, second - first - 1);
    m_ = std::stoi(month);

    std::string year = sv.substr(second + 1, sv.size());
    y_ = std::stoi(year);
}

std::string Date::to_string() {
    return std::format("{:02d}/{:02d}/{:04d}", d_, m_, y_);
}