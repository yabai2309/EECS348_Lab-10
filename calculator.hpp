#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP

#include <memory>
#include <string>

class Expression {
public:
    virtual double evaluate() = 0;
};

using ExprPtr = std::shared_ptr<Expression>;

ExprPtr parse_expression(const std::string &expression);

#endif // __CALCULATOR_HPP
