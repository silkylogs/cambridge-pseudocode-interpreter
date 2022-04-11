# Cambridge pseudocode interpreter reference manual, version 0.0.1

## Indentation
---
Lines are indented by three spaces to indicate that they are contained within a
statement in a previous line. Continuation lines are indented by two spaces.

An example demonstrating correct indentation:
```
FUNCTION trapeziumArea(paralellSide1:REAL, parallelSide2:REAL,
  height:REAL) RETURNS REAL
  // continuation used in line above (indentation is 2 spaces from function
  // declaration)

   // beginning of if statement (indentation is 3 spaces from function
   // declaration)
   IF parallelSide1 < 0 THEN

      // beginning of output statement (indentation is 3 spaces from IF clause)
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
This sample only demonstrates indendation for the general case. Additional
syntactic sugar for IF statements are in the relevant section of this document.




## Comments
---
Comments are preceded by two forward slashes `//`. The comment continues until
the end of the line. For multi-line comments, each line is preceded by `//`.

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




## Variables and constants
---
The language is statically typed, hence it is required to declare variables
explicitly in the source.

The syntax of declaring a variable is:
```
DECLARE <identifier> : <datatype>
```

Other examples of valid variable declarations include:
```
DECLARE Counter : INTEGER
DECLARE TotalToPay : REAL
DECLARE GameOver : BOOLEAN
```

Constants are declared in a similar way, however, they cannot be the value of a
variable (Implementation wise, they could be treated as an #include directive
in c to simply copy and paste the value in place of the identifier)
```
CONSTANT <identifier> = <literal value>
```

Other examples of valid constant declarations include:
```
CONSTANT HourlyRate = 6.50
CONSTANT DefaultText = "N/A"
```




## Basic datatypes
---
The basic datatypes to be supported by the compiler include:

- `INTEGER`	a whole number
- `REAL`	a number capable of containing a fractional part
- `CHAR`	a single character
- `STRING`	a sequence of zero or more characters
- `BOOLEAN`	the logical values TRUE and FALSE
- `DATE`	a valid calendar date

Any variable which is redeclared will have their values initalized.




## Literals
---
A more detailed description, as well as the representation of their
corresponding literals are as follows:

- Integers are signed whole numbers, written as normal in the denary system,
  e.g. `5`, `-3`
- Reals are signed numbers with a fractional part whose literal is always has
  at least one digit on either side of the decimal point, zeros being added if
  necessary, e.g. `4.7`, `0.3`, `-4.0`, `0.0`
- Chars are single characters delimited by single quotes e.g. `'x'`,`'C'`,`'@'`
- Strings are sequences of zero or more characters delimited by double quotes.
  A string may contain no characters (i.e. the empty string)
  e.g. `"This is a string"`, `""`
- Boolean are the logical values `TRUE` and `FALSE`
- Dates are valid calendar dates, which will be written in the format
  `dd/mm/yyyy`. (Internally, they could be represented by a struct with three
  values for date, month and year, and could be handled specially by the
  `INPUT` and `OUTPUT` commands)




## Identifiers
---
Identifiers (the names given to variables, constants, procedures and functions)
are in mix case. They can only contain letters `A–Z, a–z`, digits `0–9` and the
underscore character `_`. They may not start with a digit.

Keywords listed in the "Reserved keywords and operators" section of the
documentation may not be used as variables. Identifiers are case insensitive,
for example, `Countdown` and `CountDown` may not be used as separate variables.




## Assignment
---
The operator for assignment is a arrow pointing left `<-`, and are to be made
in the following format:
```
<identifier> <- <value>
```

The identifier must refer to a variable (this can be an individual element in a
data structure such as an array or an abstract data type). The value may be any
expression that evaluates to a value of the same data type as the variable.

Valid examples of assignments include:
```
Counter <- 0
Counter <- Counter + 1
TotalToPay <- NumberOfHours * HourlyRate
```




## Arrays
---
Arrays are considered to be fixed-length structures of elements of identical
data type, accessible by consecutive index (subscript) numbers. 
It is nessecary to state the lower bound of the array to either settle for a
0 based or a 1 based indexing system (or of any other base, should the
programmer wish to enter a code obfuscation contest.) Square brackets are used
to indicate array indices.

A One-dimensional array is declared as follows:
```
DECLARE <identifier> : ARRAY[<lower>:<upper>] OF <data type>
```

A two-dimensional array is declared as follows:
```
DECLARE <identifier> : ARRAY[<lower1>:<upper1>,<lower2>:<upper2>] OF <datatype>
```
Arrays may not have a dimension count greater than two.

Valid examples of array declaration are:
```
DECLARE StudentNames : ARRAY[1:30] OF STRING
DECLARE NoughtsAndCrosses : ARRAY[1:3,1:3] OF CHAR
DECLARE SavedGame : ARRAY[1:3,1:3] OF CHAR
```

The data of an array can be copied over to another array, provided that they
have the same size and data type:
```
SavedGame <- NoughtsAndCrosses
```

Array elements can be individually accessed by using the following construct:
```
StudentNames[1] <- "Ali"
NoughtsAndCrosses[2,3] <- ꞌXꞌ
StudentNames[n+1] <- StudentNames[n]
```

However, statements like these which accesses a group of array elements in
this explicit fashions are invalid:
```
StudentNames[1 TO 30]: <- ""
```

Instead, use an appropriate loop structure to assign the elements individually.
For example:
```
FOR Index <- 1 TO 30
   StudentNames[Index] <- ""
NEXT Index
```




## User defined datatypes
---
The `TYPE` keyword can be overloaded to mean different things, depending on the
context.

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
The following symbols are used for relational operators (also known as comparison operators):
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
The operands and results of these operations are always of data type `BOOLEAN`.
Parentheses may be used to make order of operations explicit, as the boolean
expression is generally evaluated left to right.




## String operations
---
The compiler shall provide these builtin functions (and one operator) for basic string manipulation. However, it should be noted that they can be replicated in pseudocode pretty easiliy:
```
// RIGHT(ThisString : STRING, x : INTEGER) RETURNS STRING
// Returns rightmost x characters from ThisString
RIGHT("ABCDEFGH", 3) //returns "FGH"

FUNCTION RIGHT(ThisString : STRING, x : INTEGER) RETURNS STRING
	DECLARE strLen : INTEGER
	strLen <- LENGTH(ThisString)

	IF x <= 0
		THEN
		RETURN ""
	ELSEIF x >= strLen
		RETURN ThisString
	ENDIF

	DEFINE rightMostChars : ARRAY[1:x] OF STRING	
	DEFINE thisStrIndex : INTEGER
	DEFINE subStrIndex : INTEGER

	subStrIndex <- 1
	FOR thisStrIndex <- 1 TO strLen
		IF thisStrIndex >= x
			THEN
			rightMostChars[subStrIndex] <- ThisString[thisStrIndex]
		ENDIF
	NEXT thisStrIndex
	RETURN rightMostChars
ENDFUNCTION
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
// Returns the character value representing the lower case equivalent of ThisChar.
// If ThisChar is not an upper-case alphabetic character, it is returned unchanged.
LCASE('W') //returns 'w'
```
```
// UCASE(ThisChar : CHAR) RETURNS CHAR
// Returns the character value representing the upper case equivalent of ThisChar
// if ThisChar is not a lower case alphabetic character, it is returned unchanged.
UCASE('h') //returns 'H'
```
The operator for concatenation is `&`. An example of valid usage:
```
 OUTPUT "Summer" & " " & "Pudding" //outputs "Summer Pudding"
```




## Selection statements (to be revamped)
---

### IF statements
`IF` statements may or may not have an `ELSE` clause.
`IF` statements without an else clause are written as follows:
```
IF
	<condition>
	THEN
		<statement(s)>
ENDIF
```
`IF` statements with an `ELSE` clause are written as follows:
```
IF <condition>
	THEN
	<statement(s)>
ELSE
	<statement(s)>
ENDIF
```

An example of nested `IF` statements:
```
IF ChallengerScore > ChampionScore
	THEN
	IF ChallengerScore > HighestScore
		THEN
		OUTPUT ChallengerName, " is champion and highest scorer"
	ELSE
		OUTPUT Player1Name, " is the new champion"
	ENDIF
ELSE
	OUTPUT ChampionName, " is still the champion"
	IF ChampionScore > HighestScore
		THEN
		OUTPUT ChampionName, " is also the highest scorer"
	ENDIF
ENDIF
```

### CASE statements
`CASE` statements allow one out of several branches of code to be executed, depending on the value of a variable.  
`CASE` statements are written as follows:
```
// Blank line(s) after each value are optional, and in this section
// are present for readabliliy only
// Also, yes, having the statements in a new line after the <values>
// will be enforced as I dont want to deal with indentation ambiguities

CASE OF <identifier>
	<value 1> :
		<statement1>
		<statement2>
		...

	<value 2> :
		<statement1>
		<statement2>
		...
		...
ENDCASE
```
An OTHERWISE clause can be the last case:
```
CASE OF <identifier>
	<value 1> :
		<statement1>
		<statement2>
		...

	<value 2> :
		<statement1>
		<statement2>
		...

	OTHERWISE :
		<statement1>
		<statement2>
		...
ENDCASE
```
Each value (assuming its an integer or a character being tested) may be represented by a range, for example:
```
// See how neat this is?
<value1> TO <value2> :
	<statement1>
	<statement2>
	...
```
Note that the case clauses are tested in sequence. When a case that applies is found, its statement is executed and the CASE statement is complete. Control is passed to the statement after the ENDCASE. Any remaining cases are not tested.  
If present, an `OTHERWISE` clause must be the last case. Its statement will be executed if none of the preceding cases apply.
An example of case statements in action:
```
INPUT Move
CASE OF Move
	ꞌWꞌ : 	Position <- Position − 10
	ꞌSꞌ : 	Position <- Position + 10
	ꞌAꞌ : 	Position <- Position − 1
	ꞌDꞌ : 	Position <- Position + 1
	OTHERWISE : 	CALL Beep
ENDCASE
```




## Iterators
---

### FOR loops
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


