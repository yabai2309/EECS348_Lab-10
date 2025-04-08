#include <gtest/gtest.h>

#include "calculator.hpp"

TEST(CalculatorImplementation, PlaceholderTest) {
    auto expr = parse_expression("1");
    EXPECT_NEAR(expr->evaluate(), 1.0, 0.001);
}
