#pragma once

#include <print>
#include <unordered_map>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>

void ltrim(std::string &s);
void trim(std::string &s);
void rtrim(std::string &s);
void lower(std::string &s);

template<typename T> std::string to_string(T &v) {
    std::ostringstream ss;
    ss << v;
    return ss.str();
}

template<typename T> T from_string(std::string &str) {
    std::istringstream ss(str);
    T ret;
    ss >> ret;
    return ret;
}