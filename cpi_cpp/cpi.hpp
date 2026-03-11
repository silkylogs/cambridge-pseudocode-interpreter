#pragma once

#include "util.hpp"

struct Integer {
    Integer(std::string);
    std::string to_string();
    int data_;
};

struct Real {
    Real(std::string);
    std::string to_string();
    float data_;
};

struct Char {
    Char(std::string);
    std::string to_string();
    char data_;
};

struct String {
    String(std::string);
    std::string to_string();
    std::string data_;
};

struct Boolean {
    Boolean(std::string);
    std::string to_string();
    bool data_;
};

struct Date {
    Date(std::string);
    std::string to_string();
    int d_;
    int m_;
    int y_;
};

struct Datatype {
    void *handle_to_data_type;
};

enum struct PrimitiveDataType {
    Integer, Real, Char, String, Boolean, Date,
};

struct Value {
    std::variant<Integer, Real, Char, String, Boolean, Date> data;
    PrimitiveDataType type;
};

struct Identifier {
    Identifier(std::string_view);
    std::string as_string;
};

struct Variable {
    Identifier identifier;
    Value value;
};

enum struct FileMode { Read, Write, Append, Random };
enum struct ParamPassType { Byref, Byval, };

struct Statement {
};

struct Interpreter {
    void comment(std::string comment);
    void decl_var(Identifier identifier, Datatype type);
    void decl_const(Identifier identifier, Value value);
    void assign(Identifier identifier, Value &value);
    void decl_arr(Identifier identifier, Integer l1, Integer u1, std::optional<Integer> l2, std::optional<Integer> u2, Datatype type);
    void defn_custom_type(Identifier identifier, std::vector<std::tuple<Identifier, Datatype>> data_collection);
    void input(Identifier identifier);
    void output(std::vector<Value> values);
    Value addition(Value l, Value r);
    Value subtraction(Value l, Value r);
    Value multiplication(Value l, Value r);
    Real division(Value l, Value r);
    Integer mod(Value v);
    Integer div(Integer v);
    Boolean greater_than(Value l, Value r);
    Boolean lesser_than(Value l, Value r);
    Boolean greater_than_or_equal_to(Value l, Value r);
    Boolean lesser_than_or_equal_to(Value l, Value r);
    Boolean equal_to(Value l, Value r);
    Boolean not_equal_to(Value l, Value r);
    Boolean op_and(Boolean l, Boolean r);
    Boolean op_or(Boolean l, Boolean r);
    Boolean op_not(Boolean operand);
    Integer randombetween(Integer min, Integer max);
    Real rnd();
    void stmt_if(Boolean condition, std::vector<Statement> stmts, std::vector<Statement> else_stmts);
    void stmt_case(Identifier identifier, std::vector<std::tuple<Value, Statement>>, std::optional<Statement> otherwise);
    void stmt_for(Identifier identifier, Value value1, Value value2, std::optional<Value> increment, std::vector<Statement> stmts);
    void stmt_repeat_until(std::vector<Statement> stmts, Identifier condition);
    void stmt_while(Identifier condition, std::vector<Statement> stmts);
    void defn_procedure(Identifier identifier, std::optional<std::vector<std::tuple<ParamPassType, Identifier, Datatype>>> params, std::vector<Statement> stmts);
    void call_procedure(Identifier identifier, std::optional<std::vector<Value>> values);
    void defn_function(Identifier identifier, std::optional<std::vector<std::tuple<ParamPassType, Identifier, Datatype>>> params, Datatype returns, std::vector<Statement> stmts);
    void call_function();
    void openfile(std::string file_identifier, FileMode mode);
    void readfile(std::string file_identifier, Variable &var);
    void eof(std::string file_identifier);
    void writefile(std::string file_identifier, String string);
    void closefile(std::string file_identifier);
    void seek(std::string file_identifier, Integer address);
    void getrecord(std::string file_identifier, Variable &var);
    void putrecord(std::string file_identifier, Variable &var);
};