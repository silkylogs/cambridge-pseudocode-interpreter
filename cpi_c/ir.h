// IR - Intermediate representation
//
// # Statement types
// - Declarations
// - Assignments
// - Branching
// - Loops
// - Definitions
// - Commands
//
// # Declarations
// Every variable or constant has a name. Therefore, the name can be used as a
// key to map it's value. Declarations dont come with assignments.
// - Variable
// - Constant
// - Array1D
// - Array2D
//
// # Assignments
// To simplify, this VM will not evaluate expressions. That is the job of the
// parser. The VM will instead evaluate a simpler form, such as "res" = "arg1"
// op "arg2" (except for the unary operator)
// ## Operations
// Every number is a double. Or a double with it's fractional part truncated and
// masquerading as an integer.
// - Addition    |--------------- Has lower predecence
// - Subtraction |
// - Multiplication |------------ Has higher predecence
// - Division       |
// - Greater than             |-- Has lowest predecence
// - Lesser than              |
// - Greater than or equal to |
// - Lesser than or equal to  |
// - Not equal to             |
// - Equal to                 |
// - Logical AND              |
// - Logical OR               |
// - Logical NOT              |
//
// # Branching
// if (condition) instr_ptr += offset;
// - If
// - If else
// - Case
// - Case otherwise
//
// # Loops
// - For
// - For with step
// - Repeat until
// - While
//
// # Definitions
// - Custom type. "foo.bar.baz" could be a key. Wait, everything's a double or a
// string.
// - Procedure (returns nothing)
// - Function (returns something)
//
// # Commands
// - Call function
// - Input... A num/string.
// - Output... A string. Interpreter's job to do the parsing.
// - File: open, read, write, close, seek
// - Record: get, put

// Example programs:

//
// --------------------
//
// DECLARE Counter : INTEGER
// DECLARE TotalToPay : REAL
// DECLARE GameOver : BOOLEAN
//
// --------------------
//
// CONSTANT HourlyRate = 6.50
// CONSTANT DefaultText = "N/A"
//
// --------------------
//
// Counter  0
// Counter  Counter + 1
// TotalToPay  NumberOfHours * HourlyRate
//
// --------------------
//
// DECLARE StudentNames : ARRAY[1:30] OF STRING
// DECLARE NoughtsAndCrosses : ARRAY[1:3,1:3] OF CHAR
//
// --------------------
//
// StudentNames[1]  "Ali"
// NoughtsAndCrosses[2,3]  ꞌXꞌ
// StudentNames[n+1]  StudentNames[n]
// SavedGame  NoughtsAndCrosses
//
// INPUT Answer
// OUTPUT Score
// OUTPUT "You have ", Lives, " lives left"
//
// --------------------
//
// TYPE Student
//   DECLARE Surname : STRING
//   DECLARE FirstName : STRING
//   DECLARE DateOfBirth : DATE
//   DECLARE YearGroup : INTEGER
//   DECLARE FormGroup : CHAR
// ENDTYPE
//
// ---------------------
//
// DECLARE Pupil1 : Student
// DECLARE Pupil2 : Student
// DECLARE Form : ARRAY[1:30] OF Student
//
// Pupil1.Surname     <- "Johnson"
// Pupil1.Firstname   <- "Leroy"
// Pupil1.DateOfBirth <- 02/01/2005
// Pupil1.YearGroup   <- 6
// Pupil1.FormGroup   <- ꞌAꞌ
// Pupil2             <- Pupil1
//
// FOR Index <- 1 TO 30
//   Form[Index].YearGroup <- Form[Index].YearGroup + 1
// ENDFOR Index
//
//OPENFILE StudentFile.Dat FOR RANDOM
//         FOR Position = 20 TO 10 STEP -1
//              SEEK StudentFile.Dat, Position
//              GETRECORD StudentFile.Dat, Pupil
//              SEEK StudentFile.Dat, Position + 1
//              PUTRECORD StudentFile.Dat, Pupil
//ENDFOR
//         SEEK StudentFile.Dat, 10
//         PUTRECORD StudentFile.Dat, NewPupil
//         CLOSEFILE StudentFile.dat
//
// --------------------
//
// IF ChallengerScore > ChampionScore
//   THEN
//     IF ChallengerScore > HighestScore
//       THEN
//         OUTPUT ChallengerName, " is champion and highest scorer"
//       ELSE
//         OUTPUT Player1Name, " is the new champion"
//     ENDIF
//   ELSE
//     OUTPUT ChampionName, " is still the champion"
// 	   IF ChampionScore > HighestScore
//       THEN
//         OUTPUT ChampionName, " is also the highest scorer"
//   ENDIF
// ENDIF
//
// ---------------------
//
// Total = 0
// FOR Row = 1 TO MaxRow
//   RowTotal = 0
//   FOR Column = 1 TO 10
//     RowTotal <- RowTotal + Amount[Row,Column]
//   ENDFOR Column
//   OUTPUT "Total for Row ", Row, " is ", RowTotal
//   Total <- Total + RowTotal
// ENDFOR Row
// OUTPUT "The grand total is ", Total
//
//
// ---------------------
//
// WHILE Number > 9 DO Number  Number – 9
// ENDWHILE
// ---------------------
//
//  PROCEDURE DefaultSquare
//    CALL Square(100)
//  ENDPROCEDURE
//  PROCEDURE Square(Size : integer)
//    FOR Side = 1 TO 4
//      MoveForward Size
//      Turn 90
//    ENDFOR
//  ENDPROCEDURE
//  IF Size = Default
//    THEN
//      CALL DefaultSquare
//    ELSE
//      CALL Square(Size)
//  ENDIF
//
// ---------------------
//
// FUNCTION Max(Number1:INTEGER, Number2:INTEGER) RETURNS INTEGER
//   IF Number1 > Number2
//     THEN
//       RETURN Number1
//     ELSE
//       RETURN Number2
//   ENDIF
// ENDFUNCTION
// OUTPUT "Penalty Fine = ", Max(10,Distance*2)
//
// ---------------------
//
// // This procedure swaps
// // values of X and Y
// PROCEDURE SWAP(BYREF X : INTEGER, Y : INTEGER)
//   Temp <- X // temporarily store X
//   X <- Y
//   Y <- Temp
// ENDPROCEDURE
//
// ---------------------
//
//DECLARE LineOfText : STRING
//            OPENFILE FileA.txt FOR READ
//            OPENFILE FileB.txt FOR WRITE
//            WHILE NOT EOF(FileA.txt) DO
//                 READFILE FileA.txt, LineOfText
//                 IF LineOfText = ""
//THEN
//WRITEFILE FileB.txt, "-------------------------"
//                   ELSE
//                     WRITEFILE FILEB.txt, LineOfText
//                 ENDIF
//            ENDWHILE
//            CLOSEFILE FileA.txt
//            CLOSEFILE FileB.txt
//
//
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef IR_H
#define IR_H

typedef char *OwnedCstr;
typedef char *BorrowedCStr;

struct ParamDeclVar {
    OwnedCstr name;
    OwnedCstr type;
};

struct ParamDeclVar param_decl_var_new(OwnedCstr name, OwnedCstr type);
void param_decl_var_free(struct ParamDeclVar *);

struct ParamDeclConst {
    OwnedCstr name;
    OwnedCstr type;
};

struct ParamDeclArr1d {
    OwnedCstr name;
    OwnedCstr type;
    size_t bound_lower;
    size_t bound_upper;
};

struct ParamDeclArr2d {
    OwnedCstr name;
    OwnedCstr type;
    size_t dim0_bound_lower;
    size_t dim0_bound_upper;
    size_t dim1_bound_lower;
    size_t dim1_bound_upper;
};

// This should be a variable. Probably.
typedef char *TodoType;

// The operation type (add, sub, etc) should be derived from the kind of
// operation.
struct ParamAssignment {
    TodoType result;
    TodoType param0;
    TodoType param1;
};

// "If condition is true, execute this *reigon* of code. Else skip over that
// reigon."
struct ParamBrIf {
    bool condition;
    size_t if_reigon_len;
    // if (foo) ... else ... [The else conditoin cannot
    // exist without the if]
    size_t else_reigon_len;
};

struct ParamBrCase {
    TodoType condition;
    size_t reigon_len;

    TodoType *values;
    size_t *value_statement_starts;
    size_t *value_statement_lengths;
    size_t value_cnt;
};

struct ParamLoopFor {
    OwnedCstr identifier_name;
    OwnedCstr value0_name;
    OwnedCstr value1_name;
    OwnedCstr step_name;
    size_t reigon_len;
};

struct ParamLoopRepeatUntil {
    OwnedCstr condition_name;
    size_t reigon_len;
};

struct ParamLoopWhile {
    OwnedCstr condition_name;
    size_t reigon_len;
};

struct ParamDefnCustomType {
    OwnedCstr name;
    OwnedCstr type;
};

struct ParamDefnProcedure {
    OwnedCstr name;
    size_t reigon_len;
};

struct ParamDefnFunction {
    OwnedCstr name;
    OwnedCstr ret_value_name;
    size_t reigon_len;
};

struct ParamCmdCall {
    OwnedCstr name;
};

struct ParamCmdInput {
    OwnedCstr name;
};

struct ParamCmdOutput {
    OwnedCstr str;
};

struct ParamCmdFileOpen {
    OwnedCstr identifier;
    OwnedCstr mode;
};

struct ParamCmdFileRead {
    OwnedCstr identifier;
    OwnedCstr variable;
};

struct ParamCmdFileWrite {
    OwnedCstr identifier;
    OwnedCstr filename;
};

struct ParamCmdFileClose {
    TodoType identifier;
};

struct ParamCmdFileSeek {
    OwnedCstr identifier;
    TodoType address;
};

struct ParamCmdRecordGet {
    OwnedCstr identifier;
    TodoType variable;
};

struct ParamCmdRecordPut {
    OwnedCstr identifier;
    TodoType variable;
};

struct Instr {
    enum Kind {
        DECL_VAR,
        DECL_CONST,
        DECL_ARR1D,
        DECL_ARR2D,

        ASS_ADD,
        ASS_SUB,
        ASS_MUL,
        ASS_DIV,
        ASS_GEQ,
        ASS_LEQ,
        ASS_GT,
        ASS_LT,
        ASS_EQ,
        ASS_NEQ,
        ASS_AND,
        ASS_OR,
        ASS_NOT,

        BR_IF,
        BR_IF_ELSE,
        BR_CASE,
        BR_CASE_OTHERWISE,

        LOOP_FOR,
        LOOP_FOR_STEP,
        LOOP_REPEAT_UNTIL,
        LOOP_WHILE,

        DEFN_CUSTOM_TYPE,
        DEFN_PROCEDURE,
        DEFN_FUNCTION,

        CMD_CALL,
        CMD_INPUT,
        CMD_OUTPUT,
        CMD_FILE_OPEN,
        CMD_FILE_READ,
        CMD_FILE_WRITE,
        CMD_FILE_CLOSE,
        CMD_FILE_SEEK,
        CMD_RECORD_GET,
        CMD_RECORD_PUT,
    } kind;

    // All strings are null terminated.
    // The string may encode a numeric type.
    union Parameters {
        struct ParamDeclVar decl_var;
        struct ParamDeclConst decl_const;
        struct ParamDeclArr1d decl_arr1d;
        struct ParamDeclArr2d decl_arr2d;
        struct ParamAssignment ass;
        struct ParamBrIf br_if;
        struct ParamBrCase br_case;
        struct ParamLoopFor loop_for;
        struct ParamLoopRepeatUntil loop_repeat_until;
        struct ParamLoopWhile loop_while;
        struct ParamDefnCustomType defn_custom_type;
        struct ParamDefnProcedure defn_procedure;
        struct ParamDefnFunction defn_function;
        struct ParamCmdCall call;
        struct ParamCmdInput input;
        struct ParamCmdOutput output;
        struct ParamCmdFileOpen file_open;
        struct ParamCmdFileRead file_read;
        struct ParamCmdFileWrite file_write;
        struct ParamCmdFileClose file_close;
        struct ParamCmdFileSeek file_seek;
        struct ParamCmdRecordGet record_get;
        struct ParamCmdRecordPut record_put;
    } params;
};

bool type_is_custom(struct CustomTypes *ts, char *typename);
struct Instr instr_decl_var(char const *const name, char const *const type);
void instr_free(struct Instr);
#endif
