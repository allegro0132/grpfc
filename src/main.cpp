// SA-GRPF implemented in C++
// Origin Project homepage: https://github.com/PioKow/SAGRPF
#include <iostream>
#include "analyse.h"

std::complex<double> func(const std::complex<double>&z) {
	std::complex<double> w = (z - 1.0) * std::pow((z - std::complex<double>(0, 1.0)), 2) * (z + 1.0) / (
		                         z + std::complex<double>(0, 1.0));
	return w;
}

int main() {
	// Initialization parameters
	int ItMax = 100; // Example value, set as needed
	int NodesMin = 0; // Example value
	double NodesMax = INFINITY; // Example value
	double Tol = 1e-6; // Example value
	double r = 0.5; // Initial mesh step
	double xb = -2.0;
	double xe = 2.0;
	double yb = -2.0;
	double ye = 2.0; // Domain bounds, set as needed
	// Optional parameters for fun()
	double epsilon = 0.0;

	// set up the analysis parameters
	AnalyseParams params{r, xb, xe, yb, ye, Tol, NodesMin, NodesMax, ItMax};

	// initialize the variables
	GRPFAnalyse grpf(func, params, "rect", false);
	grpf.SelfAdaptiveRun();
	// Get results
	auto res = grpf.result;
	std::cout << "Roots: " << std::endl;
	for (auto root: res.zRoots) {
		std::cout << root << std::endl;
	}
	std::cout << "Poles: " << std::endl;
	for (auto pole: res.zPoles) {
		std::cout << pole << std::endl;
	}
	return 0;
}
