#include <gtest/gtest.h>
#include <vector>
#include "grade_utils.h"

TEST(GradeUtilsTest, CalculateAverage) {
    std::vector<int> grades = {80, 90, 100};
    EXPECT_DOUBLE_EQ(average(grades), 90.0);
}

TEST(GradeUtilsTest, AverageEmptyVectorThrowsException) {
    std::vector<int> grades = {};
    EXPECT_THROW(average(grades), std::invalid_argument);
}

TEST(GradeUtilsTest, PassingGrade) {
    EXPECT_TRUE(is_passing(70));
}

TEST(GradeUtilsTest, FailingGrade) {
    EXPECT_FALSE(is_passing(69));
}

TEST(GradeUtilsTest, LetterGradeA) {
    EXPECT_EQ(letter_grade(95), 'A');
}

TEST(GradeUtilsTest, LetterGradeB) {
    EXPECT_EQ(letter_grade(85), 'B');
}

TEST(GradeUtilsTest, LetterGradeC) {
    EXPECT_EQ(letter_grade(75), 'C');
}

TEST(GradeUtilsTest, LetterGradeD) {
    EXPECT_EQ(letter_grade(65), 'D');
}

TEST(GradeUtilsTest, LetterGradeF) {
    EXPECT_EQ(letter_grade(50), 'F');
}

TEST(GradeUtilsTest, InvalidGradeThrowsException) {
    EXPECT_THROW(letter_grade(120), std::invalid_argument);
}
