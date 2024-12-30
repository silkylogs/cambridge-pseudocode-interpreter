# Cambridge Pseudocode 2017 Language specification, v0.0.1

## Abstract
This document presents a specification for a possible implementation of the 
Cambridge Pseudocode Programming language, as derived from the document titled
"Cambridge International AS & A Level Computer Science 9608 – Pseudocode Guide for Teachers", 2017.

## Grammar
A BNF for this language is as follows:
```
<statements>  ::= <statements> <eol> <statements>
              ::= <statement> | <statement_with_comment>

<statement_with_comment> ::= <statement> <comment>
<comment> ::= "//" <text> <eol>

<statement> ::= <var_decl>
            ::= <const_decl>
            ::= <array_decl>
            ::= <assignment>
            ::= <custom_type_decl>
            ::= <input>
            ::= <output>
            ::= <if_then_endif>
            ::= <if_then_else_endif>
            ::= <case_of_endcase>
            ::= <case_of_otherwise_endcase>
            ::= <for_to_endfor>
            ::= <for_to_step_endfor>
            ::= <repeat_until>
            ::= <while_do_endwhile>
            ::= <proc>
            ::= <proc_with_params>
            ::= <call_proc>
            ::= <call_proc_with_params>
            ::= <fn>
            ::= <fn_with_params>
            ::= <openfile_for_mode>
            ::= <readfile>
            ::= <writefile>
            ::= <closefile>
            ::= <seek>
            ::= <getrecord>
            ::= <putrecord>

<var_decl>    ::= "DECLARE "  <identifier> ":" <datatype> <eol>
<datatype>    ::= <identifier> | <prim_dt>
<prim_dt>     ::= "INTEGER" | "REAL" | "CHAR" | "STRING" | "BOOLEAN" | "DATE"

<const_decl>  ::= "CONSTANT " <identifier> ":" <datatype> <eol>

<array_decl>  ::= "DECLARE "  <identifier> ":" "ARRAY" "[" <array_list> "]" "OF" <datatype>
<array_list>  ::= <array_list> "," <array_list>
              ::= <expr> ":" <expr>
<expr>  ::= <expr> "AND"|"OR" <expr>
        ::= <expr> "<"|">"|"<="|">="|"="|"<>" <expr>
        ::= <expr> "+"|"-" <expr>
        ::= <expr> "*"|"/"|"MOD"|"DIV" <expr>
        ::= "NOT" <expr>
        ::= <expr> "." <expr>
        ::= "[" <expr> "]"
        ::= "(" <expr> ")"
        ::= <literal>

<literal>     ::= <int_lit> | <real_lit> | <char_lit> | <string_lit> | <bool_lit> | <date_lit>
<int_lit>     ::= <0-9>
<real_lit>    ::= <int_lit> "." <int_lit>
<char_lit>    ::= "\'"<all printable ascii chars>"\'"
<string_lit>  ::= "\""<text>"\""
<bool_lit>    ::= "TRUE" | "FALSE"
<date_lit>    ::= <int_lit>"/"<int_lit>"/"<int_lit>

<assignment>  ::= <expr> "<-" <expr>

<custom_type_decl>  ::= "TYPE " <identifier> <decl_list> "ENDTYPE"
<decl_list> ::= <decl> <eol> <decl>
            ::= <decl>
<decl>      ::= <var_decl> | <array_decl> | <const_decl> | <type_decl>


<input>   ::= "INPUT " <expr>
<output>  ::= "OUTPUT " <output_list>
<output_list> ::= <output_list> "," <output_list>
              ::= <expr>

<if_then_endif>       ::= "IF " <expr> "THEN " <statements> "ENDIF"
<if_then_else_endif>  ::= "IF " <expr> "THEN " <statements> "ELSE " <statements> "ENDIF"

<case_of_endcase>           ::= "CASE " "OF " <expr> <eol> <val_colon_statement_list> "ENDCASE"
<val_colon_statement_list>  ::= <val_colon_statement_list> <eol> <val_colon_statement_list>
                            ::= <expr> ":" <statement>

<case_of_otherwise_endcase> ::= "CASE " "OF " <expr> <eol> 
                                <val_colon_statement_list> "OTHERWISE " <statement> "ENDCASE"

<for_to_endfor>       ::= "FOR " <identifier> "<-" <expr> "TO" <expr> <statements> "ENDFOR"
<for_to_step_endfor>  ::= "FOR " <identifier> "<-" <expr> "TO" <expr> "STEP" <expr> <statements> "ENDFOR"
<repeat_until>          ::= "REPEAT " <statements> "UNTIL " <expr>
<while_do_endwhile>     ::= "WHILE " <expr> "DO " <statements> "ENDWHILE"
<proc>                  ::= "PROCEDURE " <identifier> "ENDPROCEDURE"

<proc_with_params>      ::= "PROCEDURE " <identifier> "(" <param_datatype_list> ")" <statements> "ENDPROCEDURE"
<param_datatype_list>   ::= <param_datatype_list> "," <param_datatype_list>
                        ::= <identifier> ":" <datatype>

; TODO: add byref and byval qualifiers for functions and procs
<call_proc>             ::= "CALL " <identifier>
<call_proc_with_params> ::= "CALL " <identifier> "(" <expr_list> ")"
<expr_list>             ::= <expr> "," <expr>
                        ::= <expr>

<fn>                ::= "FUNCTION " <identifier> "RETURNS " <datatype> <statements> "ENDFUNCTION"
<fn_with_params>    ::= "FUNCTION " <identifier> "(" <param_datatype_list> ")" <statements> "ENDFUNCTION"


<openfile_for_mode> ::= "OPENFILE " <identifier> "FOR " <identifier>
<readfile>  ::= "READFILE " <expr> "," <expr>
<writefile> ::= "WRITEFILE " <expr> "," <expr>
<closefile> ::= "CLOSEFILE " <expr>
<seek>      ::= "SEEK " <expr> "," <expr>
<getrecord> ::= "GETRECORD " <expr> "," <expr>
<putrecord> ::= "PUTRECORD " <expr> "," <expr>
```

## Indentation
---
The following rules apply to an autoformatter. 
This implementation will not implement continuation lines.

Lines are indented by four spaces to indicate that they are contained within
a statement in a previous line. Continuation lines are always indented by two spaces.

A continuation line is a line of code that is intended to be concatenated
to the previous line. This line is identified by a token, which is `  ` (Two spaces).

An example demonstrating correct indentation (to be updated for a) clarity
b) correctness):
```
FUNCTION trapeziumArea(paralellSide1:REAL, parallelSide2:REAL,
  height:REAL) RETURNS REAL
  // continuation used in line above (indentation is 2 spaces from function
  // declaration)

   // beginning of if statement (indentation is 3 spaces from function
   // declaration)
   IF parallelSide1 < 0 THEN

      // beginning of output statement
      // (indentation is 3 spaces from IF clause)
      OUTPUT "parallel side 1 cannot be negative"
      RETURN 0

   // end of if statement (indentation is 3 spaces from function declaration)
   ENDIF

   // beginning of another if statement (indentation is again, 3 spaces from
   // function declaration)
   IF
     parallelSide2 < 0
     THEN
     // continuation is used in the above two lines (indentation is 2 spaces
     // from IF statement)

      // beginning of output statement (indentation is 3 statements from an IF
      // clause)
      OUTPUT "parallel side 2 cannot be negative"
      RETURN 0

   // end of IF statement (indentation is 3 spaces from function declaration)
   ENDIF

   RETURN 0.5 * (parallellSide1 + parallelSide2) * height
   
ENDFUNCTION
```



## Comments
---
Comments are preceded by two forward slashes `//`. The comment continues until
the end of the line. For multi-line comments, each line is preceded by `//`.

Interpretation of code stops from when the comment token is encountered, 
up to the newline character.

An example of a program with valid commenting:
```
// this procedure swaps
// values of X and Y
PROCEDURE SWAP(BYREF X : INTEGER, Y INTEGER)
  DECLARE Temp : INTEGER
  Temp <- X // temporarily store X
  X <- Y
  Y <- Temp
ENDPROCEDURE
```

## Atomic ("Primitive") type names
---
An "atomic type" is defined as a data type from which all other data types may be constructed.

These types that will be available are:
- `INTEGER`: a whole number
- `REAL`: a number capable of containing a fractional part
- `CHAR`: a single character
- `STRING`: a sequence of zero or more characters
- `BOOLEAN`: the logical values `TRUE` and `FALSE`
- `DATE`: a valid calendar date

Their default values will be as follows:
- `INTEGER`:  `0`
- `REAL`:     `0.0`
- `CHAR`:     `' '`
- `STRING`:   `""`
- `BOOLEAN`:  `FALSE`
- `DATE`:     `1/1/1970`

## Literals
---
A literal is a textual representation of the value of an atomic data type.
The valid literals of the atomic data types will be represented as follows:

- `INTEGER`: Integers are signed whole numbers, written as normal in the denary system,
  e.g. `5`, `-3`.
- `REAL`: Reals are signed numbers with a fractional part whose literal is always has
  at least one digit on either side of the decimal point, zeros being added if
  necessary, e.g. `4.7`, `0.3`, `-4.0`, `0.0`
- `CHAR`: Chars are single characters delimited by single quotes e.g. `'x'`,`'C'`,`'@'`
- `STRING`: Strings are sequences of zero or more characters delimited by double quotes.
  A string may contain no characters (i.e. the empty string)
  e.g. `"This is a string"`, `""`
- `BOOLEAN`: Booleans are the logical values `TRUE` and `FALSE`
- `DATE`: Dates are valid calendar dates, in the format `dd/mm/yyyy`,
  where each of the date, month and year fields are `INTEGER`s.

## Identifiers
---
Identifiers (the names given to variables, constants, procedures and
functions) will only contain letters `A-Z`, `a-z` and digits `0-9`.
The first character of an identifier will not start with a digit.
They will not contain accented characters, including the underscore (`_`),
or emojis.

Keywords listed in the "Reserved keywords and operators" section of the
documentation will not be used as variables. Identifiers are case insensitive,
for example, `Countdown` and `CountDown` will not refer to separate
variables.

## Variables and constants
---
Variables may be declared explicitly before use. The syntax for doing that is:
```
DECLARE <identifier> : <datatype>
```
Examples of valid variable declarations:
```
DECLARE Counter : INTEGER
DECLARE TotalToPay : REAL
DECLARE GameOver : BOOLEAN
```

Variables may be implicitly brought to life by assigning the 
result of an expression to it, as shown in these examples:
```
Counter <- 0              // Has type INTEGER
TotalToPay <- 3.50        // Has type REAL
GameOver <- Counter = 0   // Has type BOOLEAN
```

Constants are declared by stating the identifier and the literal value
in the following format:
```
CONSTANT <identifier> = <literal value>
```

Valid examples of valid constant declarations include:
```
CONSTANT HourlyRate = 6.50    // Has type REAL
CONSTANT DefaultText = "N/A"  // Has type STRING
```

Only literals will be used as the value of a constant. A variable, 
another constant or an expression will not be used as a value of a constant.

A constant's value will not be reassigned.

Once a variable or constant is declared, implicitly or explicitly, another 
variable or constant with the same identifier, regardless of the type of that other 
variable or constant, will not be declared, explicitly or implicitly.

The lifetime of a variable or constant begins from the point where it is declared,
either implicitly or explicitly, and ends with its scope.
The lifetime of a global variable or constant begins from the point of its declaration
up to the end of the program.

A program will not attempt to reference the value of a variable whose lifetime has expired.



## Assignment
---
The operator for assignment is a arrow pointing left `<-`, and will be made
in the following format:
```
<identifier> <- <value>
```

The identifier will refer to a variable (this can be an individual element in
a data structure such as an array or an abstract data type). The value will be
any expression that evaluates to a value of the same data type as the variable.

Valid examples of assignments include:
```
Counter <- 0
Counter <- Counter + 1
TotalToPay <- NumberOfHours * HourlyRate
Applicants[Counter].Valid = TRUE
```




## Arrays >>>(TODO)<<<
---
Arrays are fixed-length structures of elements of identical data type, 
accessible by consecutive index (subscript) numbers.
Square brackets are used to indicate array indices.

A one-dimensional array is declared as follows:
```
DECLARE <identifier> : ARRAY[<lower>:<upper>] OF <data type>
```

A two-dimensional array is declared as follows:
```
DECLARE <identifier> : ARRAY[<lower1>:<upper1>,<lower2>:<upper2>] OF <datatype>
```

Valid examples of array declaration are:
```
DECLARE StudentNames : ARRAY[1:30] OF STRING
DECLARE NoughtsAndCrosses : ARRAY[1:3,1:3] OF CHAR
DECLARE SavedGame : ARRAY[1:3,1:3] OF CHAR
```

The values of the elements of the array will be initialized to the default
value of its data type.

Arrays will not have a dimension count greater than two.

Assignment of arrays will be possible, provided that they have 
the same size and data type.
```
SavedGame <- NoughtsAndCrosses
```
After the operation, the destination array will have the same data
as that of the source array.

Array elements can be individually accessed by using the following construct:
```
StudentNames[1] <- "Ali"
NoughtsAndCrosses[2,3] <- 'X'
StudentNames[n+1] <- StudentNames[n]
```
Where the accessing index will be an expression resolving to `INTEGER`.

Statements like these which accesses a group of array elements in
this particular fashion is invalid:
```
StudentNames[1 TO 30]: <- ""
```
There will be a recommendation to perform the following operation instead:
```
FOR Index <- 1 TO 30
   StudentNames[Index] <- ""
NEXT Index
```






## Custom types
---
The `TYPE` keyword means different things, depending on the context.

When declared as an enumerated type, where the datatype can have a discrete
list of possible values (the equivalent of `enum` in c):
```
TYPE <identifier> = (value1, value2, value3, ...)
TYPE Season = (Spring, Summer, Autumn, Winter) //Example
```

When declared as a pointer, which is a noncomposite datatype referencing a
memory location:
```
TYPE <pointer_name> = ^<Typename>
TYPE TAddPointer = ^INTEGER //Example
```

Pointers are dereferenced by suffixing the `^` character to it. An example:
```
TAddPointer^ <- TAddPointer^ + 1
```

When declared as a composite datatype, which is a collection of data which
consists of different data types (the equivalent of `struct` in c):
```
TYPE <identifier1>
   DECLARE <identifier2> : <data type>
   DECLARE <identifier3> : <data type>
   DECLARE <identifier_n> : <data type>
ENDTYPE

//Example:
TYPE Student
   DECLARE Surname : STRING
   DECLARE FirstName : STRING
   DECLARE DateOfBirth : DATE
   DECLARE YearGroup : INTEGER
   DECLARE FormGroup : CHAR
ENDTYPE
```

A complete example where all three types of `TYPE`s are used:
```
TYPE Student
   DECLARE Surname : STRING
   DECLARE FirstName : STRING
   DECLARE DateOfBirth : DATE
   DECLARE YearGroup : INTEGER
   DECLARE FormGroup : CHAR
ENDTYPE
TYPE Season = (Spring, Summer, Autumn, Winter)
TYPE TAddPointer = ^INTEGER

DECLARE Pupil1 : Student
DECLARE Pupil2 : Student
DECLARE Form : ARRAY[1:30] OF Student
DECLARE ThisSeason : Season
DECLARE NextSeason : Season
DECLARE MyAddPointer : TAddPointer

Pupil1.Surname <- "Johnson"
Pupil1.Firstname <- "Leroy"
Pupil1.DateOfBirth <- 02/01/2005
Pupil1.YearGroup <- 6
Pupil1.FormGroup <- ꞌAꞌ

Pupil2 <- Pupil1 // Copying over data between two custom composite datatypes

FOR Index ← 1 TO 30
   Form[Index].YearGroup <- Form[Index].YearGroup + 1
NEXT INDEX // Note that identifiers are case insensitive

ThisSeason <- Spring
MyAddPointer <- ^ThisSeason
NextSeason <- MyAddPointer^ + 1
// Pointer is dereferenced to access the value stored at the address
```




## Input/Output
---
In pseudocode, I/O operations are done with the `INPUT` and `OUTPUT` commands
respectively.

The input command only accepts one variable of a basic datatype as input
```
INPUT <value>
```

The output command accepts multiple comma seperated values, or literals, of
basic datatypes (Internally, the values could be evaluated, then concatated to
one single output string)
```
OUTPUT <value1>, <value2>, ... , <value_n>
```

Valid examples of I/O commands
```
INPUT Answer
OUTPUT Score
OUTPUT "You have ", Lives, " lives left"
```




## Arithmetic operations
---
Standard arithmetic operator symbols are used:
- `+`	Addition
- `-`	Subtraction
- `*`	Multiplication
- `/`	Division
The resulting value of the division operation will be of datatytpe `REAL`,
regardless wether the operands were integers. Multiplication and division have
higher precedence over addition and subtraction. Parentheses may be used to
make order of operations explicit.




## Relational operations
---
The following symbols are used for relational operators (also known as
comparison operators):
- `>`	Greater than
- `<`	Less than
- `>=`	Greater than or equal to
- `<=`	Less than or equal to
- `=`	Equal to
- `<>`	Not equal to
The result of these operations is always of data type `BOOLEAN`.
Parentheses may be used to make order of operations explicit, as the boolean
expression is generally evaluated left to right.




## Logic operators
---
- `AND`	Logical and
- `OR`	Logical or
- `NOT`	Logical not
The operands and results of these operations are always of data type
`BOOLEAN`. Parentheses may be used to make order of operations explicit, as
the boolean expression is generally evaluated left to right.




## String operations
---
The specification shall provide these builtin functions (and one operator) for
basic string manipulation.
```
// RIGHT(ThisString : STRING, x : INTEGER) RETURNS STRING
// Returns rightmost x characters from ThisString
RIGHT("ABCDEFGH", 3) //returns "FGH"
```

```
// LENGTH(ThisString : STRING) RETURNS INTEGER
// Returns the integer value representing the length of ThisString
LENGTH("Happy Days") //returns 10 
```

```
// MID(ThisString : STRING, x : INTEGER, y : INTEGER) RETURNS STRING
// Returns string of length y starting at position x from ThisString
MID("ABCDEFGH", 2, 3) //returns "BCD"
```

```
// LCASE(ThisChar : CHAR) RETURNS CHAR
// Returns the character value representing the lower case equivalent of
// ThisChar. If ThisChar is not an upper-case alphabetic character, it is
// returned unchanged.
LCASE('W') //returns 'w'
```

```
// UCASE(ThisChar : CHAR) RETURNS CHAR
// Returns the character value representing the upper case equivalent of 
// ThisChar if ThisChar is not a lower case alphabetic character, it is
// returned unchanged.
UCASE('h') //returns 'H'
```

The operator for concatenation is `&`. An example of valid usage:
```
 OUTPUT "Summer" & " " & "Pudding" //outputs "Summer Pudding"
```


## Random number generation
---
The following builtin functions are provided for pseudorandom random number
generation:
```
RANDOMBETWEEN(min,max)	// generates a random integer between the integers min and max
RND()			// generates a random real number between 0 and 1. 
```


## IF statements
---
`IF` statements may or may not have an `ELSE` clause.

`IF` statements without an `ELSE` clause are as follows:
```
IF <condition>		// 0 spaces from base
  THEN			// 2 spaces from base
    <statements>	// 4 spaces from base
ENDIF			// 0 spaces from base
```

`IF` statements with an `ELSE` clause are as follows:
```
IF <condition>		// 0 spaces from base
  THEN			// 2 spaces from base
    <statements>	// 4 spaces from base
  ELSE			// 2 spaces from base
    <statements>	// 4 spaces from base
ENDIF			// 0 spaces from base
```
Note that the `THEN` and `ELSE` clauses are only indented by two spaces. (They
are, in a sense, a continuation of the `IF` statement rather than separate
statements).

When `IF` statements are nested, the nesting should continue the indentation
of two spaces starting from the above `THEN` statement.

An example of nested `IF` statements with the _recommended*_ usage of
indentation:
```
IF challengerScore > championScore		// 0 spaces from base
  THEN						//   2
    IF challengerScore > highestScore		//     4
      THEN					//       6
        OUTPUT challengerName, " is champion"	//         8
      ELSE					//       6
        OUTPUT player1Name, " is new champ"	//         8
    ENDIF					//     4
  ELSE						//   2
    OUTPUT championName, " is still the champ"	//     4
    IF championScore > HighestScore		//     4
      THEN					//       6
        OUTPUT championName, " is also highest"	//         8
    ENDIF					//     4
ENDIF						// 0
```

While the indentation style is recommended for purposes of easier reading,
this wont be enforced as the specification will consider the next lines
indented with two spaces as continuation lines. They will be
appended to the previous lines before any further processing.

Thus, enforcing the rule would result in the lines being interpreted as
follows:
```
IF challengerScore > championScore THEN			// 0 spaces from base
    IF challengerScore > highestScore THEN		//     4
        OUTPUT challengerName, " is champion" ELSE	//         8
        OUTPUT player1Name, " is new champ"		//         8
    ENDIF ELSE						//     4 
    OUTPUT championName, " is still the champ"		//     4
    IF championScore > HighestScore THEN		//     4
        OUTPUT championName, " is also highest"		//         8
    ENDIF						//     4
ENDIF							// 0
```                                                     
Henceforth, the "Lines are indented by four spaces to indicate that they are
contained within a statement in a previous line" rule holds for IF statements.
                                                        
                                                        



## CASE statements
`CASE` statements allow one out of several brances of code to be executed,
depending on the value of a variable. An `OTHERWISE` clause can be the last
case, which is executed only when none of the preceding cases apply.

`CASE` statements are as follows:
```
CASE OF <identifier>
  <value 1> : <statement>
  <value 2> : <statement>
  <value n> : <statement>
ENDCASE
```
Each case clause is indented by two spaces, as they are seen as the
continuation of the `CASE` statement rather than new statements.

As a result, the branches of code to be executed will only contain single
statements.

The case clauses are tested in sequence. When a case that applies is found,
its statement is executed and the `CASE` statement is complete. Control is
passed to the statement after the `ENDCASE`. Any remaining cases are not
tested. If present, an `OTHERWISE` clause must be the last case. Its statement
will be executed if none of the preceding cases apply.

An example of a validly formatted CASE statement:
```
INPUT move
CASE OF move
  'w': position <- position + 10
  's': position <- position - 10
  'a': position <- position + 1
  'd': position <- position - 1
  OTHERWISE: beep
ENDCASE
```

For the implementor, the following statements would get transformed into the
following:
```
INPUT move
CASE OF move 'w': position <- position + 10 's': position <- position - 10 'a': position <- position + 1 'd': position <- position - 1 OTHERWISE: beep
ENDCASE
```
Each statement will be delimited by the letter of a new case, which in turn is
determined by the colon character.






## FOR loops (to be revamped)
---
The syntax of this loop is as follows:
```
FOR <identifier> <- <value1> TO <value2> // STEP will default to 1 in this case
	<statement(s)>
NEXT <identifier>
```
```
FOR <identifier> <- <value1> TO <value2> STEP <increment>
	<statement(s)>
NEXT <identifier>
```
The identifier must be a variable of data type `INTEGER`, and the values should be expressions that evaluate to integers. The variable is assigned each of the integer values from `value1` to `value2` inclusive, running the statements inside the FOR loop after each assignment. If `value1 = value2` the statements will be executed once, and if `value1 > value2` the statements will not be executed. (Reminder: any variables redeclared in the loop will have their values initalized.)

If the `STEP` keyword is used, `increment` must be an expression that evaluates to a signed integer. In this case the identifier will be assigned the values from `value1` in successive increments of `increment` until it reaches `value2.` If it goes past `value2`, the loop terminates.

An example of a program with a nested `FOR` loop:
```
// Assume the constant MaxRow and the array Amount have been declared elsewhere
DECLARE Total : INTEGER
Total <- 0
 
FOR Row ← 1 TO MaxRow
	DECLARE Rowtotal : INTEGER
	RowTotal <- 0
	
	FOR Column <- 1 TO 10
		RowTotal <- RowTotal + Amount[Row, Column]
	NEXT Column
	
	OUTPUT "Total for Row ", Row, " is ", RowTotal
	Total ← Total + RowTotal
NEXT Row
OUTPUT "The grand total is ", Total
```


### REPEAT loops
`REPEAT` or "repeat-until" or postcondition loops are written as follows:
```
REPEAT
	<Statement(s)>
UNTIL <condition>
```
The condition must be an expression that evaluates to `BOOLEAN`. The statements in the loop will be executed at least once. The condition is tested after the statements are executed and if it evaluates to `TRUE` the loop terminates, otherwise the statements are executed again.

A syntatically valid example of a repeat-until loop:
```
REPEAT
	DECLARE Passphrase : STRING
	OUTPUT "Please enter the passphrase"
	INPUT Passphrase
UNTIL Passphrase = "Why the fuck is the passphrase is stored as a plaintext string constant"
```


### WHILE loops
The syntax of `WHILE` or precondition loops is as follows:
```
WHILE <condition>
	<statement(s)>
ENDWHILE
```
The condition must be an expression that evaluates to `BOOLEAN`. It is tested before the statements, and the statements will only be executed if the condition evaluates to `TRUE`. After the statements have been executed the condition is tested again. The loop terminates when the condition evaluates to `FALSE`. The statements will not be executed if, on the first test, the condition evaluates to `FALSE`.

A simple example of a while loop:
```
DECLARE Number : INTEGER
Number <- 99
WHILE Number > 9
	Number <- Number – 9
ENDWHILE
```




## Functions
---
Functions are a sequence of program instuctions which will return a single value.

A function without parameters is defined as:
```
FUNCTION <identifier> RETURNS <data type>
	<statement(s)>
ENDFUNCTION
```
A function with parameters is defined as:
```
FUNCTION <identifier>(<param1> : <datatype>, ... ) RETURNS <data type>
	<statement(s)>
ENDFUNCTION
```
The keyword `RETURN` is used as one of the statements within the body of the function to specify the value to be returned. Normally, this will be the last statement in the function definition. There may be multiple return statements in a function definition, however the flow of execution in the function will end upon reaching the first `RETURN` statement.  
**Functions should not be called by the `CALL` statement.**

A valid example of a function in use:
```
FUNCTION Max(Number1:INTEGER, Number2:INTEGER) RETURNS INTEGER
	IF Number1 > Number2
		THEN
		RETURN Number1
	ELSE
		RETURN Number2
	ENDIF
ENDFUNCTION

OUTPUT "Penalty Fine = ", Max(10, Distance*2)
```

### Parameter input types
The compiler will allow programmers to specify, in a function or a procedure, wether to pass the parameter by value (a copy of the parameter variable is used rather than the actual parameter) or pass it by reference (the parameter variable itself is subject to change in the function, and will retain any value once  the function has been executed, similar to prepending a parameter with the character `&` in c++). These functionality can be accessed by the keywords `BYVAL` and `BYREF` respectively.

An example of `BYREF` in action:
```
PROCEDURE SWAP(BYREF X : INTEGER, Y : INTEGER)
	DECLARE Temp : INTEGER
	Temp <- X
	X <- Y
	Y <- Temp
ENDPROCEDURE
```

A C - like implementation of the same thing, without the `BYREF` keyword:
```
PROCEDURE SWAP(X : ^INTEGER, Y : ^INTEGER)
	DECLARE Temp : INTEGER
	Temp <- ^X
	X <- Y
	^Y <- Temp
ENDPROCEDURE
```

**Note**
> While the official Cambridge International AS & A Level Computer Science 9618 Teacher's Pseudocode Guide expressly forbids passing parameters by reference to any given function, given that enforcing this rule will result in slower programs, the compiler will support this functionality, however issuing a warning similar to: `Nonstandard syntax warning: The 9618 pseudocode guide expressly forbids passing parameters by reference to a function`



## Procedures
---
A procedure is a sequence of program instructions which will be executed where `identifier` is called. Procedures do not return a value.

A procedure without parameters is defined as follows:
```
PROCEDURE <identifier>
	<statement(s)>
ENDPROCEDURE
```
A procedure with parameter(s) is defined as follows:
```
PROCEDURE<identifier>(<param1> : <datatype>, <param2> : <datatype>, ... )
	<statement(s)>
ENDPROCEDURE
```

Procedures are called by the following syntax:
```
CALL <identifier>
```
```
CALL <identifier>(Value1, Value2, ... )
```

When parameters are used, `Value1, Value2, ...` must be of the correct data type and in the same
sequence as in the definition of the procedure, as expected of a general programming language.  
**Unless otherwise stated, it should be assumed that parameters are passed by value.**

An example of procedures used with and without parameters, as well as calling other procedures:
```
PROCEDURE MoveForward(Steps : INTEGER)
	OUTPUT "The turtle moved ", Steps, " Step(s)"
ENDPROCEDURE

PROCEDURE Turn(Angle : REAL)
	OUTPUT "The turtle turned an angle of ", Angle, " degrees"
ENDPROCEDURE

PROCEDURE DefaultSquare
	CALL Square(100)
ENDPROCEDURE

PROCEDURE Square(Size : INTEGER)
	FOR Side ← 1 TO 4
		CALL MoveForward(Size)
		CALL Turn(90)
	NEXT Side
ENDPROCEDURE


IF Size = Default
	THEN
	CALL DefaultSquare
ELSE
	CALL Square(Size)
ENDIF
```




## File handling
---

### Handling text files
Text files consist of lines of text that are read or written consecutively as strings. A file must be opened in a specified mode before any file operations are attempted.

The syntax for opening a file in such a way is:
```
OPENFILE <File identifier> FOR <File mode>
```

`File identifier` may be a literal string containing the file names, or a variable of type `STRING` that has been assigned the file name.  
The following file modes are used:

- `READ`
For data to be read from the file

- `WRITE`
For data to be written to the file. A new file will be created and any existing data in the
file will be lost.

- `APPEND`
For data to be added to the file, after any existing data.

- `RANDOM`
For opening files in random access mode

A file may be opened in only one mode at a time, else an error is thrown.  
Data is read from the file (after the file has been opened in READ mode) using the READFILE command as follows:
```
READFILE <File Identifier>, <Variable>
```

`Variable` should be of data type `STRING`. When the command is executed, the next line of
text in the file is read and assigned to the variable.

The function `EOF` is used to test whether there are any more lines to be read from a given file. It is called as follows:
```
EOF(<File Identifier>)
```

This function returns `TRUE` if there are no more lines to read (or if an empty file has been opened in `READ` mode) and `FALSE` otherwise.

Data is written into the file (after the file has been opened in `WRITE` or `APPEND` mode) using the `WRITEFILE` command as follows:
```
WRITEFILE <File identifier> , <data>
```

Files should be closed when they are no longer needed using the `CLOSEFILE` command as
follows:
```
CLOSEFILE <File identifier>
```

An example to demonstrate file handling operations with the commands shown:
```
// This copies the contents of FileA.txt to FileB.txt

DECLARE LineOfText : STRING
OPENFILE "FileA.txt" FOR READ
OPENFILE "FileB.txt" FOR WRITE

WHILE NOT EOF("FileA.txt")
	READFILE "FileA.txt", LineOfText
	IF LineOfText = ""
		THEN
		WRITEFILE "FileB.txt", "-------------------------"
	ELSE
		WRITEFILE "FILEB.txt", LineOfText
	ENDIF
ENDWHILE

CLOSEFILE "FileA.txt"
CLOSEFILE "FileB.txt"
```


### Handling random files
Random files contain a collection of data, normally as records of fixed length. They can be thought of as having a file pointer which can be moved to any location or address in the file. The record at that location can then be read or written.

Random files are opened using the `RANDOM` file mode as follows:
```
OPENFILE <File identifier> FOR RANDOM
```

As with text files, the file identifier will normally be the name of the file. The `SEEK` command moves the file pointer to a given location:
```
SEEK <File identifier>, <address>
```

The address should be an expression that evaluates to `INTEGER` which indicates the location of a record to be read or written. This is usually the number of records from the beginning of the file.

The command `GETRECORD` should be used to read the record at the file pointer:
```
GETRECORD <File identifier>, <Variable>
```

When this command is executed, the variable is assigned to the record that is read, and must be of the appropriate data type for that record (usually a user-defined type).

The command `PUTRECORD` is used to write a record into the file at the file pointer:
```
PUTRECORD <File identifier>, <Variable>
```

When this command is executed, the data in the variable is inserted into the record at the file pointer. Any data that was previously at this location will be replaced.

An example of handling random files:
```
// The records from (arbitpositions 10 to 20 of a file StudentFile.Dat are moved to the next position and a new record is inserted into position 10.
// The Student datatype is the very same datatype defined previously in the "User defined datatypes" section

DECLARE Pupil : Student
DECLARE NewPupil : Student
DECLARE Position : INTEGER

NewPupil.Surname <- "Johnson"
NewPupil.Firstname <- "Leroy"
NewPupil.DateOfBirth <- 02/01/2005
NewPupil.YearGroup <- 6
NewPupil.FormGroup <- ꞌAꞌ

OPENFILE StudentFile.Dat FOR RANDOM

FOR Position = ← 20 TO 10 STEP -1
	SEEK "StudentFile.Dat", Position
	GETRECORD "StudentFile.Dat", Pupil
	SEEK "StudentFile.Dat", Position + 1
	PUTRECORD "StudentFile.Dat", Pupil
NEXT Position

SEEK "StudentFile.Dat", 10
PUTRECORD "StudentFile.Dat", NewPupil

CLOSEFILE "StudentFile.dat"
```



## A complete list of reserved keywords and operators
---
### Operators:
```
-
<-
*
/
//
+
<
<=
<>
=
>
>=
&
```
### Keywords:
```
AND
APPEND
ARRAY
BOOLEAN
BYREF
BYVAL
CALL
CASE OF
CHAR
CLOSEFILE
CONSTANT
DATE
DECLARE
DIV
ELSE
ENDCASE
ENDFUNCTION
ENDIF
ENDPROCEDURE
ENDTYPE
EOF
FALSE
FOR ... TO
FUNCTION
GETRECORD
IF
INPUT
INTEGER
LCASE
LENGTH
MID
MOD
NEXT
NOT
OPENFILE
OR
OTHERWISE
OUTPUT
PROCEDURE
PUTRECORD
READ
READFILE
REAL
REPEAT
RETURN
RETURNS
RIGHT
RND
SEEK
STEP
STRING
THEN
TRUE
TYPE
UCASE
UNTIL
WHILE
WRITE
WRITEFILE
```


