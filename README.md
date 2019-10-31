C-Python Translator and C-MIPS compiler
=======================================

The program(including compiler and testbench) is built using command:

    make all

C-Python translator invoked using command:

    ./bin/c_compiler --translator [input_file.c] -o [output_file.py]

C-MIPS compiler invoked using command:

    ./bin/c_compier --S [input_file.c] -o [output_file.s]

C-MIPS compiler testbench invoked using command

    ./testbench
    
Input C code should be well-formed

Compiler Supported Constructs
-----------------------------------
Funtion types:
- int: with zero or multiple arguments
- int main: with zero or multiple arguments
- void: with zero or multiple arguments

Variable types:
- local int
- global int

Arithmetic operators:

    +, -, *, /, %

Bitwise operators:

    |, &, ^

Comparison operatos:

    ==, <, >

Assignment operaor:
    
    =

Logical operators:

    ||, && 
    Note that both left and right side should be a binary comparison expression

Variable declaration:
- int a; (by our default and concern of consistency, save zero value, making equivalent to "int a=0";)
- int a=3;(assign a constant on declaration)
- int a=b;(assign with another variable on declaration)
- int a=b+c;(assign a binary expression)
- int a=b+3;(assign a binary expression with a constant ON THE RIGHT, constant on the RIGHT!)
- Declaration list supported (a compound statement of declaration in all forms above)
- Global variable supported

Assgiment:
- All variables should be ALREADY DECLARED!!
- a=0;(assign a variable with 0, move $0 to dest reg)
- a=3;(assign a variable with a constant)
- a=a+b;(assign a variable with a binary expression)
- a=b+3;(assign a variable with a binary expression with constant, constant on the RIGHT!)
- Assignment list supported (a compound statement of assignment in all forms above)
- Global variable supported

Return statement:
- Assuming functions well-formed!(NO return in void, and return MUST exist in int)
- return 0;(return zero value, move $0 to $2)
- return 3;(return non-zero value, li $2 with constant)
- return a;(return a variable)
- return a+b;(return a binary expression)
- return a+3;(return a binary expression with constant, constant on the RIGHT!)
- Global variable supported

Conditional statements: 
- if:conditions support comparison and logical operators
- if-else:same note as above
- while:same note as above

Additional notes:
- For all expressions(arithmetic, comprison, assigment, bitwise) involving a constant, put the constant on the RIGHT!! (use a+3, not 3+a)
- Global variables are supported in all arithmetic, bitwise, assignment and comparison expressions and return statement
- To name a variable or a function, use ONLY lower and/or higher case English letters
- So far only support binary expressions, if want to use a=b+c+d, use a=b+c; a=a+d;

Translator Supported Constructs
---------------------------------------
Funtion types:
- int: with zero or multiple arguments
- int main: with zero or multiple arguments
- void: with zero or multiple arguments

Variable types:
- local int
- global int(with no assignment)

Arithmetic operators:
- All arithmetic operators in standard C are supported

Bitwise operators:
- All bitwise operators in standard C are supported

Comparison operators:
- All comparison operators in standard C are supported

Logical operators:
- All logical operators in standard C are supported

Assignment operators:
- All assignment operators(=, +=, -= and so on) in standard C are supported

Iteration operators:
- ++
- \-\-

Declarations:
- Pure declaration
- Assignment declaration

Statements:
- return
- if
- if-else
- while

Additional Notes & Acknowledgement
-----------------------------------
- Unsolved bug: when compiling (T_VARIABLE T_MINUS T_NUMBER), will return syntax error, but (T_VARIABLE T_MINUS T_VARIABLE) works well, and all other operators work well
- Yacc grammar reference:  https://www.lysator.liu.se/c/ANSI-C-grammar-y.html


