#include "calculator.hpp"
#include <cassert>
#include <iostream>

//проверка правильности вычисления элементарных выражений
void expression_tests_evaluate_tests()
{
    std::cout << "start evaluation tests:" << std::endl;
    double value_1{2.33}, value_2{3.33};
    expression first_expr{"2.33"}, second_expr{"3.33"};
    assert(double(2.33) == expression("2.33").evaluate());
    assert(double(2.33) == expression("2,33").evaluate());
    assert(double(-3.33) == expression("-", expression("3.33")).evaluate());
    assert(value_1 + value_2 == expression("+", first_expr, second_expr).evaluate());
    assert(value_1 - value_2 == expression("-", first_expr, second_expr).evaluate());
    assert(value_1 * value_2 == expression("*", first_expr, second_expr).evaluate());
    assert(value_1 / value_2 == expression("/", first_expr, second_expr).evaluate());
    std::cout << "evaluation tests OK!\n";
}

//тест возникновения исключения при некорректном значении элементарного выражения
void expression_tests_exception_tests() {
    std::cout << "start exception test\n";
    bool isException;
    try {
        expression("something...").evaluate();
        isException = false;
    } catch (std::runtime_error &e) {
        isException = true;
    }
    assert(isException);


    std::cout << "exception test OK!\n";
}

//тесты парсера унарных выражений
void parser_tests_Unary_parser_test(){
    std::cout << "start unary expression parser test:"<< std::endl;
    assert(expression("-", expression("2.33")) == parser{"-2.33"}.parse());
    assert(expression("-",expression("2,33")) == parser{"-2,33"}.parse());
    assert(expression("2.33") == parser{"2.33"}.parse());
    assert(expression("-", expression("2.33")) == parser{"-      2.33"}.parse());
    std::cout << "unary expression parser test OK!" << std::endl;
}

//тесты возникновения исключений при некорректных значениях унарного выражения
void parser_tests_Unary_parser_exceptions_test() {
    std::cout << "start unary parser exceptions test:\n";
    bool isException;
    try {
        parser{"something..."}.parse();
        isException = false;
    } catch (std::runtime_error &e) {
        isException = true;
    }
    assert(isException);

    try {
        parser{"*6"}.parse();
        isException = false;
    } catch (std::runtime_error &e) {
        isException = true;
    }
    assert(isException);

    try {
        parser{"/6"}.parse();
        isException = false;
    } catch (std::runtime_error &e) {
        isException = true;
    }
    assert(isException);

    try {
        parser{"+6"}.parse();
        isException = false;
    } catch (std::runtime_error &e) {
        isException = true;
    }
    assert(isException);


    std::cout << "unary parser exception tests OK!\n";
}

//тесты парсера бинарного выражения
void parser_tests_binary_parser_tests(){
    std::cout << "start binary parser tests:\n";
    assert(expression("+", expression("2.33"), expression("3.33")) == parser("2.33+3.33").parse());
    assert(expression("-", expression("2.33"), expression("3.33")) == parser("2.33-3.33").parse());
    assert(expression("*", expression("2.33"), expression("3.33")) == parser("2.33*3.33").parse());
    assert(expression("/", expression("2.33"), expression("3.33")) == parser("2.33/3.33").parse());

    assert(expression("+", expression("-", expression("2.33")), expression("3.33")) == parser("-2.33+3.33").parse());

    //строка с пробелами
    assert(expression("+", expression("2.33"), expression("-",expression("3.33"))) == parser("2.33       +   -  3.33").parse() );
    std::cout << "binary parser tests OK!\n";
}

//тест обработки множественного выражения
void parser_tests_multiple_expressions_test(){
    std::cout << "start multiple expression tests: \n";
    assert(expression("+", expression("*", expression("2.33"), expression("3.33")),
            expression("/", expression("2.33"), expression("3.33"))) == parser("2.33*3.33+2.33/3.33").parse());

    std::cout << "multiple expressions test OK!\n";
}

//тест работы со скобками
void parser_tests_brackets_test() {
    std::cout << "start brackets test\n";
    assert(expression("*", expression("+", expression("2.33"), expression("3.33")),
            expression("-", expression("2.33"), expression("3.33"))) == parser{"(2.33+3.33)*(2.33-3.33)"}.parse());

    std::cout << "brackets test OK!\n";
}


void calculator_test(std::string input, double expected_val){
    try{
        parser p{input};
        auto result{ p.parse().evaluate() };
        assert(result == expected_val);
    } catch (std::runtime_error &e) {
        std::cout << e.what() << "\n";
        assert(false);
    }
}

//общие тесты
void calculator_tests() {
    calculator_test("-1 + 5 - 3", 1);
    calculator_test("-10 + (8 * 2.5) - (3 / 1.5)", 8);
    calculator_test("1 + (2*(2.5+2.5+(3-2)))-(3/1.5)", 11);

}

int main() {
    setlocale(LC_ALL, "Russian");


    std::cout << "start expression tests:\n\n";
    expression_tests_evaluate_tests();
    expression_tests_exception_tests();
    std::cout << "\nAll expression tests OK!\n\n";


    std::cout << "start parser tests:\n\n";
    parser_tests_Unary_parser_test();
    parser_tests_Unary_parser_exceptions_test();
    parser_tests_binary_parser_tests();
    parser_tests_multiple_expressions_test();
    parser_tests_brackets_test();
    std::cout << "\nAll parser tests OK!\n\n";

    std::cout << "start calculator tests:\n";
    calculator_tests();
    std::cout << "calculator tests OK!\n\n";

    std::cout << "All test OK!\n";


    return 0;
}

