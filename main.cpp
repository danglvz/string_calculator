#include "calculator.hpp"
#include <iomanip>
#include <iostream>

int main(int argc, char** argv){
    setlocale(LC_ALL, "Russian");

    std::cout << "Введите выражение:\n";
    std::string input_string;
    std::cin >> input_string;

    parser p{input_string};
    try {
        auto result{ p.parse().evaluate() };
        std::cout << std::fixed;
        std::cout << std::setprecision(2) <<std::endl;
        std::cout << result << std::endl;
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

