#include "util.hpp"

void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
};

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
};

void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
};

void lower(std::string &s) {
    std::transform(
        s.begin(), s.end(),
        s.begin(),
        [](char c) -> char {
        if (c <= 'Z' && c >= 'A')
            return c - ('Z' - 'z');
        return c;
    }
    );
};