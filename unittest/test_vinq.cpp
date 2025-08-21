//
// Created by Ziang on 2025/8/20.
//#include <gtest/gtest.h>
#include <complex>
#include "utils.h"

TEST(VinqFunction, Returns1ForFirstQuadrant) {
	std::complex<double> input(1.0, 1.0);
	EXPECT_EQ(grpfc::vinq(input), 1);
}

TEST(VinqFunction, Returns2ForSecondQuadrant) {
	std::complex<double> input(-1.0, 1.0);
	EXPECT_EQ(grpfc::vinq(input), 2);
}

TEST(VinqFunction, Returns3ForThirdQuadrant) {
	std::complex<double> input(-1.0, -1.0);
	EXPECT_EQ(grpfc::vinq(input), 3);
}

TEST(VinqFunction, Returns4ForFourthQuadrant) {
	std::complex<double> input(1.0, -1.0);
	EXPECT_EQ(grpfc::vinq(input), 4);
}

TEST(VinqFunction, ReturnsNegative1ForOrigin) {
	std::complex<double> input(0.0, 0.0);
	EXPECT_EQ(grpfc::vinq(input), -1);
}

TEST(VinqFunction, Returns1ForPositiveRealAxis) {
	std::complex<double> input(1.0, 0.0);
	EXPECT_EQ(grpfc::vinq(input), 1);
}

TEST(VinqFunction, Returns2ForPositiveImaginaryAxis) {
	std::complex<double> input(0.0, 1.0);
	EXPECT_EQ(grpfc::vinq(input), 2);
}

TEST(VinqFunction, Returns3ForNegativeRealAxis) {
	std::complex<double> input(-1.0, 0.0);
	EXPECT_EQ(grpfc::vinq(input), 3);
}

TEST(VinqFunction, Returns4ForNegativeImaginaryAxis) {
	std::complex<double> input(0.0, -1.0);
	EXPECT_EQ(grpfc::vinq(input), 4);
}