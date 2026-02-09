#include <print>
#include <unordered_map>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <variant>

#include "util.hpp"

using Data = std::variant<int64_t, std::vector<unsigned char>>;
struct VarData {
    std::string type;
    Data data;
};
using VarsInScope = std::unordered_map<std::string, VarData>;
using Scopes = std::vector<VarsInScope>;

const std::string name_type_sep = ":";
const std::string adt_integer = "integer";

static void exec_stmt(VarsInScope &vars, std::string stmt) {
    trim(stmt);

    std::string first_word;
    auto first_space = stmt.find(" ");
    first_word = stmt.substr(0, first_space);
    lower(first_word);

    if ("declare" == first_word) {
        auto colon = stmt.find(name_type_sep);
        std::string var_name = stmt.substr(first_space, colon - first_space);
        trim(var_name);
        lower(var_name);
        if (vars.contains(var_name)) {
            std::print("Variable \"{}\" declared previously in this scope.\n", var_name);
            exit(0);
        }

        std::string var_type = stmt.substr(colon + name_type_sep.size(), stmt.size());
        trim(var_type);
        lower(var_type);

        auto default_val_from_type = [](std::string var_type) -> Data {
            assert(adt_integer == var_type);
            {
                int64_t num = 0;
                Data dat{ num };
                return dat;
            }
        };
        Data default_value = default_val_from_type(var_type);

        VarData v{ var_type, default_value };
        vars.insert(std::make_pair(var_name, v));
    } else if ("output" == first_word) {
        constexpr auto space_sep = std::string(" ").size();
        auto output_var = stmt.substr(first_space + space_sep, stmt.size());
        trim(output_var);

        // TODO: Parse output

        if (auto search = vars.find(output_var); search != vars.end()) {
            auto &v = search->second;
            std::visit([](auto &&arg) { std::print("{}", arg); }, v.data);
        } else {
            std::print("Variable \"{}\" not found in this scope\n", output_var);
            assert(0);
        }
    } else { // Assignment
        assert(!"TODO");
    }
}

// TODO: Implement tests. Every branch of this code.

int main() {
    VarsInScope dat;
    exec_stmt(dat, "declare foo: integer");
    exec_stmt(dat, "declare bar: integer");

    exec_stmt(dat, "foo = 1");
    exec_stmt(dat, "bar <- 2");

    exec_stmt(dat, "output foo");
    exec_stmt(dat, "output bar");
}
