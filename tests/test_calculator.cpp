#include <gtest/gtest.h>
#include "calculator.h"

TEST(CalculatorTest, AddPositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
}

TEST(CalculatorTest, AddNegativeNumbers) {
    EXPECT_EQ(add(-2, -3), -5);
}

TEST(CalculatorTest, SubtractNumbers) {
    EXPECT_EQ(subtract(10, 4), 6);
}

TEST(CalculatorTest, MultiplyNumbers) {
    EXPECT_EQ(multiply(3, 4), 12);
}

TEST(CalculatorTest, DivideNumbers) {
    EXPECT_EQ(divide(10, 2), 5);
}

TEST(CalculatorTest, DivideByZeroThrowsException) {
    EXPECT_THROW(divide(10, 0), std::invalid_argument);
}

TEST(CalculatorTest, DetectEvenNumber) {
    EXPECT_TRUE(is_even(8));
}

TEST(CalculatorTest, DetectOddNumber) {
    EXPECT_FALSE(is_even(7));
}
