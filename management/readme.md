Reflection
========================

This reflection explains our process in building the compiler project, in terms of our making and change in decisions, _major_and_macroscopic_ milestones, as well as what we have learnt from this.


Startup decisions
-------------------

The whole compiler started with deciding what subset of C to implement.

At first, we tried to build the whole standard C, including array, pointer, and other key words such as static, extern.

Later we found it was not at all plausible for a group of two in one month.

Although it wasted like 4 or 5 days, we found it useful in terms of planning lexer and parser, like lexer tokens and parser node hierarchy.

Finally, we decided to implement only a subset of standard C as in the spec.


Building the lexer
-------------------

We started with lexer,strictly following regex matching convention as in the lectures, for example, to match "++", we use [+]{2}, but later we discovered that this was not very efficient and prone to error.

Since the compiler constructs, except characters and numbers, do not contain any over-complicated patterns that is really necessary to generalise, we simply use "" to match everything instead.


Building the parser
------------------------

After prototyping the lexer, we started working on parser.

In our opinion, the parser was the hardest bit in the whole project, since the parser was the foundation for everything that follows.

Once we have a good structure of parser, it would make much more sense to build the translator and compiler. 

Therefore, we spent much time on constructing the parser, and by the time of the informal pull, we were still trying to perfect out parser.
(As seen in the effort-log and milestone files, it took us more than one week to do this)

We used a standard C Yacc grammar website as a reference(quoted in Acknowledgement in the main readme.md). We tried to understand how the flow runs in that parser, and we simplified it while adding other features to make it our own.


Now for AST
-----------------

With lexer and parser almost done, we started to decide AST classes.

This process was not that difficult, as the parser has already had its structure, but there was still a lot work to do. We needed to decide in each action, whether it should instantiate a new class or recur to another node.

For the AST class, we decided to separate into 3 top level categories: expressions(everything that means a value), statements(everything that ends with a semicolon) and declaration(function and variable declaration)

We started with expressions(arithmetic, logical, comparison and so on), as these were the most obvious ones to implement, and then statements and declaration.


Making things linked
---------------------

The next challenge was to link everything together, making the tool chain run, namely, the main and the makefile.

We have to admit that this action was quite risky, as we did not develop things step by step, but wanted to make the whole thing run in one shot, and we did encounter lots of bugs, some of which were robust, such as parser hierarchy issues(a small change could change everything)

This was another hardest time in the whole project, and again, we did not make physical improvements in several days. Could be seen in the effort-log and milestone files, we spent another huge amount of time in this. (Absolutely a lesson!) 


Finally translator and compiler
--------------------------------

The good thing was that after debugging, the translator worked pretty well, proving that the parser worked well, except some indentation issues, which were solved in AST classes in several hours.

Now things started to get clear, as we physically understood how things flow in the language processor. With the remaining time of two weeks, we started to work on the compiler.

Although, in terms of workload, compiler was much heavier than translator, since we have a physical understanding of the tool chain, things follow pretty linearly, and we could make progress on a regular basis in writing the compiler.


What we have learnt
---------------------

We learnt to use flex and yacc to build lexer and parser for a compiler, as well as the command to compile it.

In writing AST classes, we had a better understanding of recursion functions and pointers, and we realised how powerful pointers are, especially in cases where lots of inheritance hierarchy exists.

In writing the test suite, we learnt to use command to compile c into mips assembly, linking object files together and run in qemu mips, and this explains what does an actual compiler do with the source code.

Now we acquired knowledge of compilation in terms of lexing, parsing and code generation, and compiler has been not a black box from now.

In terms of project management, we realised that it was pretty risky to put everything together, hoping it to run in one time. If one step was wrong, and we followed the wrong convention, in the worst case, we need to re-write everything.
