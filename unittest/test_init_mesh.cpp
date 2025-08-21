//
// Created by Ziang on 2025/8/19.
//
#include <iostream>
#include "meshing.h"

int main() {
	// Example usage of rect_dom
	double xb = -1.0, xe = 1.0, yb = -2.0, ye = 5.0, r = 0.1;
	Eigen::MatrixXd mesh = rect_dom(xb, xe, yb, ye, r);
	std::cout << "Rectangular Domain Mesh:\n" << mesh << std::endl;

	// Example usage of disk_dom
	// double radius = 1.0;
	// int num_points = 100;
	// Eigen::MatrixXd disk_mesh = disk_dom(radius, num_points);
	// std::cout << "Disk Domain Mesh:\n" << disk_mesh << std::endl;

	return 0;
}
