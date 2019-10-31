CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include

all : compiler testbench

compiler : bin/c_compiler

testbench : test_deliverable/test

src/parser.tab.cpp src/parser.tab.hpp : src/parser.y
	bison -v -d src/parser.y -o src/parser.tab.cpp

src/lexer.yy.cpp : src/lexer.flex src/parser.tab.hpp
	flex -o src/lexer.yy.cpp src/lexer.flex

bin/c_compiler.o : c_compiler.cpp
	mkdir -p bin
	g++ $(CPPFLAGS) -c c_compiler.cpp -o bin/c_compiler.o

bin/c_compiler : bin/c_compiler.o src/parser.tab.o src/lexer.yy.o src/parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/c_compiler $^

test_deliverable/test : test_deliverable/test_all.cpp
	mkdir -p test_deliverable/mips_out
	g++ test_deliverable/test_all.cpp -o testbench


clean : 
	rm bin/*
	rm src/*.o
	rm src/*.tab.cpp
	rm src/*.yy.cpp
	rm src/*.output
	rm src/*.tab.hpp
	rm testbench
	rm test_deliverable/mips_out/*
































