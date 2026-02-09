#include <print>
#include <unordered_map>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>

#include "util.hpp"

using Data = std::vector<unsigned char>;
struct Variable {
    std::string type;
    Data data;
};
using ScopeVars = std::unordered_map<std::string, Variable>;
//using Scopes = std::vector<ScopeVars>;
//Scopes scopes;

// Parse program
// - determine stmt type
// - execute stmt
static void exec_stmt(ScopeVars &vars, std::string stmt) {
    trim(stmt);

    auto first_space = stmt.find(" ");
    std::string first_word = stmt.substr(0, first_space);
    lower(first_word);

    if ("declare" == first_word) {
        auto colon = stmt.find(":");
        auto count = colon - first_space;
        std::string var_name = stmt.substr(first_space, count);
        trim(var_name);
        lower(var_name);
        if (vars.contains(var_name)) {
            std::print("Variable \"{}\" declared previously in this scope.\n", var_name);
            exit(0);
        }

        std::string var_type = stmt.substr(colon + 1, stmt.size());
        trim(var_type);
        lower(var_type);

        auto default_val_from_type = [](std::string var_type) -> Data {
            assert("integer" == var_type);
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
        vars.insert(std::make_pair(var_name, v));
    } else if ("output" == first_word) {
        auto output_var = stmt.substr(first_space + 1, stmt.size());
        trim(output_var);

        // TODO: Parse output

        if (auto search = vars.find(output_var); search != vars.end()) {
            auto v = search->second;
            assert("integer" == v.type);
            {
                int64_t val = *reinterpret_cast<int64_t *>(v.data.data());
                std::print("{}", val);
            }
        } else {
            std::print("Variable \"{}\" not found in this scope\n", output_var);
            assert(0);
        }
    } else { // Assignment
        std::string asssignment_operator = "=";

        auto ass_op_loc = stmt.find(asssignment_operator);
        std::string dest = stmt.substr(0, ass_op_loc);
        trim(dest);
        lower(dest);
        
        if (auto search = vars.find(dest); search != vars.end()) {
            std::string expr = stmt.substr(ass_op_loc + asssignment_operator.size(), stmt.size());
            trim(expr);

            auto val = from_string<int64_t>(expr);
            auto &dest = search->second;
            dest.data.clear();
            dest.data.resize(sizeof(int64_t));
            memcpy(dest.data.data(), &val, sizeof(int64_t));
        } else {
            std::print("Assign: variable \"{}\" not found in this scope\n", first_word);
            assert(0);
        }
    }
}

int main() {
    ScopeVars dat;
    exec_stmt(dat, "declare foo: integer");
    exec_stmt(dat, "foo = 3");
    exec_stmt(dat, "output foo");
}
