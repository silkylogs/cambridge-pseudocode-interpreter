#include <print>
#include <map>
#include <vector>
#include <string>
#include <cassert>

using Data = std::vector<unsigned char>;
struct Variable {
    std::string type;
    Data data;
};
using ScopeVars = std::map<std::string, Variable>;
//using Scopes = std::vector<ScopeVars>;
//Scopes scopes;

auto ltrim = [](std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
};

auto rtrim = [](std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
};

auto trim = [](std::string &s) {
    ltrim(s);
    rtrim(s);
};

// Parse program
// - determine stmt type
// - execute stmt
static void exec_stmt(ScopeVars &vars, std::string stmt) {
    trim(stmt);

    auto first_space = stmt.find(" ");
    std::string first_word = stmt.substr(0, first_space);

    if ("DECLARE" == first_word) {
        auto colon = stmt.find(":");
        auto count = colon - first_space;
        std::string var_name = stmt.substr(first_space, count);
        trim(var_name);

        std::string var_type = stmt.substr(colon + 1, stmt.size());
        trim(var_type);

        auto default_val_from_type = [](std::string var_type) -> Data {
            assert("INTEGER" == var_type);
            {
                int64_t num = 0;
                Data dat;
                dat.resize(sizeof(int64_t));
                memcpy(dat.data(), &num, sizeof(int64_t));
                return dat;
            }
        };
        Data default_value = default_val_from_type(var_type);

        Variable v{ var_type, default_value };
        vars[var_name] = v;
    } else if ("OUTPUT" == first_word) {
        auto output_var = stmt.substr(first_space + 1, stmt.size());
        trim(output_var);

        // TODO: Parse output

        if (auto search = vars.find(output_var); search != vars.end()) {
            auto v = search->second;
            assert("INTEGER" == v.type);
            {
                int64_t val = *reinterpret_cast<int64_t *>(v.data.data());
                std::print("{}", val);
            }
        } else {
            std::print("Variable \"{}\" not found in this scope\n", output_var);
            assert(0);
        }
    }
}

int main() {
    ScopeVars dat;
    exec_stmt(dat, "DECLARE Foo : INTEGER");
    exec_stmt(dat, "OUTPUT Foo");
}
