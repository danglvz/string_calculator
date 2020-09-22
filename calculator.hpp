

#ifndef CALCULATOR_CALCULATOR_HPP
#define CALCULATOR_CALCULATOR_HPP

// Парсер арифмитеческих выражений реализован с помощью метода "рекурсивного спуска".
//Основная идея этого метода заключается, в том чтобы разнести рарзбор разных операций
//(с разным приоритетом) по разным взаимно-рекурсивным функциям :
//1) токенизатор (разбирает входную строку на короткие последовательности, в данном сучае на
// арифметические опереции и на числа)
//2) парсер унарных выражений, например: "-1", 1.
//3) парсер бинарных выражений, например: "1 + 2", "2 * 6", "2 * (1+5)".


#include <string>
#include <utility>
#include <vector>
#include <stdexcept>


//вектор хранящий все возможные токены
const std::vector<std::string> tokens{"+", "-", "*", "/", "(", ")"};

//элементарное выражение
class expression {
    std::string token_;
    std::vector<expression> args; //аргументы выражения
public:
    //конструктор для чисел
    explicit expression(std::string token) : token_{std::move(token)} {}
    //конструктор для унарного выражения
    explicit expression(std::string token, expression right) : token_{std::move(token)}, args{std::move(right)} {}
    //конструктор для бинарного выражения
    explicit expression(std::string token, expression left, expression right) : token_{std::move(token)}, args{std::move(left), std::move(right)} {}
    //вычисление выражения
    double evaluate();
    //сравнение двух выражений
    bool operator==(const expression &expr) const;

};

//парсер входящей строки
class parser {
    const std::string &str; //ссылка на входящую строку
    size_t pos; //конкретная позиция в входящей строке
    std::string token_parser(); // токенизатор
    expression parse_unary_expression(); // парсер унарного выражения
    expression parse_binary_expression(int priority); // парсер бинарного выражения

public:
    explicit parser(const std::string &input) : str{input}, pos{} {}

    expression parse();
};



double expression::evaluate() {
    switch (args.size()) {
        //число
        case 0: {
            //замена всех запятых в числе на точки
            if (auto index = token_.find(',') != std::string::npos) {
                token_.replace(index, 1, ".");
            }
            double output;
            try {
                output = std::stod(token_, nullptr);
                return output;
            } catch (std::invalid_argument &e) {
                throw std::runtime_error("строка содержит недопустимое выражение: " + token_);
            }

        }
        //унарное выражение
        case 1: {
            auto val{args[0].evaluate()};
            if (token_ == "-") return -val;
            else throw std::runtime_error("строка содержит недопустимое выражение: " + token_ + args[1].token_);
        }
        //бинарное
        case 2: {
            auto left{args[0].evaluate()}, right{args[1].evaluate()};
            if ( token_ == "+" ) return left + right;
            else if ( token_ == "-" ) return left - right;
            else if ( token_ == "*" ) return left * right;
            else if ( token_ == "/" ) return  left / right;
            else throw std::runtime_error("строка содержит недопустимое выражение: " + args[0].token_ + token_ + args[1].token_ );
        }


    }
    return 0;
}

bool expression::operator==(const expression &expr) const {
    return args == expr.args && token_ == expr.token_;
}

std::string parser::token_parser() {
    for ( ;std::isspace(str[pos]); ++pos ); //пропускаем пробелы

    //если первый символ - цира, то парсим число
    if ( std::isdigit(str[pos]) ) {
        std::string value;
        for ( ;std::isdigit(str[pos]) || str[pos] == '.' || str[pos] == ','; ++pos) {
            value.push_back(str[pos]);
        }
        return value;
    }

    //парсим арифметическую операцию или скобки
    for ( auto &expect_token: tokens) {
        if (expect_token == str.substr(pos, expect_token.length())) {
            pos += expect_token.length();
            return expect_token;
        }
    }

    return  "";

}

expression parser::parse_unary_expression() {
    auto current_token{token_parser()};

    //если неизвестный токен, то выбрасываем исключение
    if ( current_token.empty() ) throw std::runtime_error("строка содержит недопустимое выражение :" + str.substr(pos));

    //если токен - число, возвращаем число
    if ( std::isdigit(current_token[0])) {
        return expression(current_token);
    } else if ( current_token == "-" ) {
        return expression(current_token, parse_unary_expression());
    }else if  ( current_token == "(" ){ //парсим выражение в скобках
        auto result = parse();
        if ( token_parser() != ")" ) throw std::runtime_error("в строке отсутсвует закрывающая скобка\n");
        return result;
    } else {
        throw std::runtime_error("строка содержит недопустимое выражение: " + str.substr(pos));
    }
}

expression parser::parse() {
    return parse_binary_expression(0);
}

expression parser::parse_binary_expression(int minimal_priority) {
    auto left_expr{parse_unary_expression()};

    for (;;) {
        auto binary_operator{ token_parser() };
        int current_priority; //приоритет операции
        if ( binary_operator == "+" ) current_priority = 1;
        else if ( binary_operator == "-" ) current_priority = 1;
        else if ( binary_operator == "*" ) current_priority = 2;
        else if ( binary_operator == "/" ) current_priority = 2;
        else current_priority = 0;

        //выходим из цикла, если приоритет слишком низкий
        if ( current_priority <= minimal_priority ) {
            pos -= binary_operator.length();
            return left_expr;
        }
        //парсим выражение справа и устанавливаем новый минимальный приоритет
        auto right_expr{ parse_binary_expression(current_priority)};
        left_expr = expression(binary_operator, left_expr, right_expr);
    }
}




#endif
