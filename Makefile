all : calculator_test calculator 

calculator: main.cpp
	g++ -std=c++17 -O2 -o calculator main.cpp

calculator_test :   calculator_tests.cpp
	g++ -std=c++17  -O2 -o  tests   calculator_tests.cpp
