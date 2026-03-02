#include "cpi.hpp"

Integer::Integer(std::string sv) : data{ std::stoi(sv) } {
}

std::string Integer::to_string() {
    return std::to_string(this->data);
}

Real::Real(std::string sv) : data{ std::stof(sv) } {
}

std::string Real::to_string() {
    return std::to_string(this->data);
}

Char::Char(std::string sv) : data { sv[0] } {
}

std::string Char::to_string() {
    std::string s = "";
    s.push_back(this->data);
    return s;
}

String::String(std::string sv) : data {sv} {
}

std::string String::to_string() {
    return this->data;
}

Boolean::Boolean(std::string sv) : data {} {
    lower(sv);
    if (sv == "true") this->data = true;
    else if (sv == "false") this->data = false;
    else throw std::invalid_argument("Cannot parse as bool");
}

std::string Boolean::to_string() {
    return std::to_string(this->data);
}
