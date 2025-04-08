#include <gtest/gtest.h>

#include "calculator.hpp"

TEST(CalculatorImplementation, PlaceholderTest) {
    double expr = parse_expression("1");
    EXPECT_NEAR(expr, 1.0, 0.001);
}
