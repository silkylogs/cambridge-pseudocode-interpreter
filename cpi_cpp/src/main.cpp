#include <print>
#include <unordered_map>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <variant>
#include <functional>

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

auto split = [](std::string s) -> std::tuple<std::string, std::string> {
    auto space = s.find(" ");
    return std::make_tuple(
        s.substr(0, space),
        s.substr(space + 1, s.size())
    );
};

auto name_colon_type = [](std::string s) -> std::tuple<std::string, std::string> {
    auto colon = s.find(":");
    return std::make_tuple(
        s.substr(0, colon),
        s.substr(colon + 1, s.size())
    );
};

static bool exec_stmt(VarsInScope &vars, std::string stmt) {
    trim(stmt);

    // First word hints at the statement type
    auto [first_word, rest] = split(stmt);
    lower(first_word);

    if ("declare" == first_word) {
        // Name : type splitter
        auto [name, type] = name_colon_type(rest);

        trim(name);
        lower(name);
        if (vars.contains(name)) {
            std::println("Variable \"{}\" declared previously in this scope.", name);
            return false;
        }

        // Create default value
        auto default_val_from_type = [](std::string var_type) -> Data {
            assert(adt_integer == var_type);
            {
                int64_t num = 0;
                Data dat{ num };
                return dat;
            }
        };
        trim(type);
        lower(type);
        Data default_value = default_val_from_type(type);

        // Assign default value
        VarData v{ type, default_value };
        vars.insert(std::make_pair(name, v));
    } else if ("output" == first_word) {
        // Get output expr
        trim(rest);

        // Eval output expr
        if (auto search = vars.find(rest); search != vars.end()) {
            auto &v = search->second;
            std::visit([](auto &&arg) { std::print("{}", arg); }, v.data);
        } else {
            std::println("Variable \"{}\" not found in this scope", rest);
            return false;
        }
    } else { // Assignment
        std::string ass_op_eq = "=";
        std::string ass_op_ar = "<-";
        std::string assop;

        auto &lhs = first_word;

        ltrim(rest);
        auto loc_eq = rest.find(ass_op_eq);
        auto loc_arrow = rest.find(ass_op_ar);
        if (loc_eq == 0) {
            assop = ass_op_eq;
        } else if (loc_arrow == 0) {
            assop = ass_op_ar;
        } else {
            std::println("Assignment operator not found at beginning of rhs");
            return false;
        }

        auto rhs = rest.substr(rest.find(assop)+assop.size(), rest.size());
        trim(rhs);
        lower(rhs);

        if (auto search_lhs = vars.find(lhs); search_lhs != vars.end()) {
            if (auto search_rhs = vars.find(rhs); search_rhs != vars.end()) {
                if (search_lhs->second.type == search_rhs->second.type) {
                    search_lhs->second.data = search_rhs->second.data;
                    return true;
                } else {
                    std::println("LHS type ({}) and RHS type ({}) do not match", search_lhs->second.type, search_rhs->second.type);
                    return false;
                }
            } else {
                std::println("RHS \"{}\" not found in this scope", rhs);
                return false;
            }
        } else {
            std::println("LHS \"{}\" not found in this scope", lhs);
            return false;
        }
    }
    return true;
}

// TODO: Implement tests. Every branch of this code.

struct Test {
    std::string name;
    std::function<bool()> fn;
};

Test tst(std::string n, std::function<bool()> fn) { return Test(n, fn); }

bool run_tests() {
    std::vector<Test> tests = {
        tst("Split", []() -> bool {
            auto [x,y] = split("DECLARE y");
            return x == "DECLARE" && y == "y";
        }),

        tst("Decl punctuation stress test", []() -> bool {
            VarsInScope dat;

            bool ok = exec_stmt(dat, "declare v00 : integer")
            && exec_stmt(dat, "DECLARE v01 : integer")
            && exec_stmt(dat, "declare v02: integer")
            && exec_stmt(dat, "DECLARE v03: integer")
            && exec_stmt(dat, "declare v04 :integer")
            && exec_stmt(dat, "DECLARE v05 :integer")
            && exec_stmt(dat, "declare v06:integer")
            && exec_stmt(dat, "DECLARE v07:integer")
            && exec_stmt(dat, "declare v08 : INTEGER")
            && exec_stmt(dat, "DECLARE v09 : INTEGER")
            && exec_stmt(dat, "declare v10: INTEGER")
            && exec_stmt(dat, "DECLARE v11: INTEGER")
            && exec_stmt(dat, "declare v12 :INTEGER")
            && exec_stmt(dat, "DECLARE v13 :INTEGER")
            && exec_stmt(dat, "declare v14:INTEGER")
            && exec_stmt(dat, "DECLARE v15:INTEGER")
            ;

            ok &= dat.size() == 16;

            for (const auto &d : dat) {
                ok &= adt_integer == d.second.type;
            }

            return ok;
        }),

        tst("Decl variable case insensitivity", []() -> bool {
            VarsInScope dat;

            bool ok0 = exec_stmt(dat, "declare variable0 : integer");
            bool ok1 = !exec_stmt(dat, "DECLARE VaRiAbLe0 : integer");
            bool ok = ok0 && ok1;

            ok &= dat.size() == 1;

            for (const auto &d : dat) {
                ok &= adt_integer == d.second.type;
            }

            return ok;
        }),

        tst("Name:Type", []() -> bool {
            auto [foo, bar] = name_colon_type("Foo:Bar");
            return foo=="Foo"&&bar=="Bar";
        }),

        tst("Assignment", []() -> bool {
            VarsInScope dat;
            bool ok = true;
            ok &= exec_stmt(dat, "declare foo: integer");
            ok &= exec_stmt(dat, "declare bar: integer");

            if (auto search = dat.find("foo"); search != dat.end()) {
                auto &data = search->second.data;
                ok &= 0 == std::get<int64_t>(data);
            } else {
                ok = false;
            }

            if (auto search = dat.find("bar"); search != dat.end()) {
                search->second.data = 3;
            } else {
                ok = false;
            }

            ok &= exec_stmt(dat, "foo = bar");
            if (auto search = dat.find("foo"); search != dat.end()) {
                auto &data = search->second.data;
                ok &= 3 == std::get<int64_t>(data);
            } else {
                ok = false;
            }

            if (auto search = dat.find("foo"); search != dat.end()) {
                search->second.data = 5;
            } else {
                ok = false;
            }

            ok &= exec_stmt(dat, "bar <- foo");
            if (auto search = dat.find("bar"); search != dat.end()) {
                auto &data = search->second.data;
                ok &= 5 == std::get<int64_t>(data);
            } else {
                ok = false;
            }

            return ok;
        }),
    };

    bool all_ok = true;
    for (size_t i = 0; i < tests.size(); ++i) {
        auto &test = tests[i];
        bool ok = test.fn();
        std::println("{} of {}: Test \"{}\"... {}", i + 1, tests.size(), test.name, ok);
        all_ok &= ok;
    }
    std::println("\nTest summary: all_ok={}", all_ok);
    return all_ok;
}

int main() {
    assert(run_tests());
}
