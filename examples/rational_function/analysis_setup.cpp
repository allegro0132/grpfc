// Analyze a rational function using GRPF algorithm
// Created by Ziang on 2025/8/19.
//
#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <cmath>
#include "../../src/utils.h"
#include "../../include/params.h"

// Example rational function
inline std::complex<double> fun(const std::complex<double> z, const double epsilon) {
	// set rotes and roles for the rational function
	std::complex<double> za(0.5, -std::sqrt(3.0) / 6.0);
	std::complex<double> zb(0.0, std::sqrt(3.0) / 3.0);
	std::complex<double> zc(-0.5, -std::sqrt(3.0) / 6.0);

	std::complex<double> w = (z - za) * (z - zb - epsilon) / (z - zc) / (z - zb + epsilon);
	return w;
}

int main() {
	// test the rational function
	std::complex<double> z_in = std::complex<double>(0.1, 0.2);
	double epsilon = 0.01;
	std::complex<double> result = fun(z_in, epsilon);
	std::cout << "Result of the rational function: " << result << std::endl;

	// set up the analysis parameters
	analysisParams params{-1.0, 1.0, -1.0, 1.0, 1e-6, 10, 1000, 50000, 1};

  // generate the initial mesh


	// Example usage of plot fun and visualization using gnuplot
	// Create a grid of complex numbers for plotting
	// auto plt = PlotUtils(-1.0, 1.0, -1.0, 1.0, 500);
	// auto dataFile = plt.getPhaseData([&](const std::complex<double> z) {
	// 	return fun(z, epsilon);
	// });
	// plt.plot(dataFile, "Phase Plot of Rational Function");


	return 0;
}
