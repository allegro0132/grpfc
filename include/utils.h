//
// Created by Ziang on 2025/8/19.
//

#ifndef GRPFC_UTILS_H
#define GRPFC_UTILS_H

#pragma once
#include <vector>
#include <complex>
#include <fstream>
#include <Eigen/Dense>
#include "CDT.h"
#include "gnuplot.h"

class PlotUtils {
public:
	float start_x, start_y;
	float end_x, end_y;
	float step_x, step_y;
	int grid_size;

	inline PlotUtils(float start_x, float end_x, float start_y, float end_y, int grid_size) {
		this->start_x = start_x;
		this->start_y = start_y;
		this->end_x = end_x;
		this->end_y = end_y;
		this->grid_size = grid_size;
		this->step_x = (end_x - start_x) / (grid_size - 1);
		this->step_y = (end_y - start_y) / (grid_size - 1);
	}

	std::string getPhaseData(const std::function<std::complex<double>(std::complex<double>)>& fun) {
		std::ofstream output_phase("phase.dat");
		for (int i = 0; i < grid_size; ++i) {
			for (int j = 0; j < grid_size; ++j) {
				auto x = start_x + i * step_x;
				auto y = start_y + j * step_y;
				auto z = std::complex<double>(x, y);
				auto val = fun(z);
				output_phase << x << " " << y << " " << std::arg(val) << std::endl;
			}
			output_phase << std::endl;
		}
		output_phase.close();
		return "phase.dat";
	}

	std::string getMagnitudeData(const std::function<std::complex<double>(std::complex<double>)>& fun) {
		std::ofstream output("mag.dat");
		for (int i = 0; i < grid_size; ++i) {
			for (int j = 0; j < grid_size; ++j) {
				auto x = start_x + i * step_x;
				auto y = start_y + j * step_y;
				auto z = std::complex<double>(x, y);
				auto val = fun(z);
				output << x << " " << y << " " << std::sqrt(std::pow(val.imag(), 2) + std::pow(val.real(), 2)) <<
						std::endl;
			}
			output << std::endl;
		}
		output.close();
		return "mag.dat";
	}


	static void plot(const std::string&dataFile, const std::string&title = "Phase Plot") {
		GnuplotPipe gp;
		gp.sendLine("set title '" + title + "'");
		gp.sendLine("set view map");
		gp.sendLine("unset surface");
		gp.sendLine("set pm3d at b");
		gp.sendLine("set palette rgbformulae 30,31,32");
		gp.sendLine("splot '" + dataFile + "' with pm3d");
	}
};

namespace grpfc {
	template<typename T>
	std::vector<T> linspace(T start, T end, int num_points) {
		std::vector<T> result(num_points);
		if (num_points == 0) {
			return result;
		}
		if (num_points == 1) {
			result[0] = start;
			return result;
		}

		T increment = (end - start) / static_cast<T>(num_points - 1);
		for (int i = 0; i < num_points; ++i) {
			result[i] = start + i * increment;
		}
		return result;
	}

	// Function to simulate meshgrid
	inline void meshgrid(const Eigen::ArrayXd& x_vals, const Eigen::ArrayXd& y_vals,
	                     Eigen::MatrixXd& X, Eigen::MatrixXd& Y) {
		long int num_x = x_vals.size();
		long int num_y = y_vals.size();

		// Initialize X and Y arrays with appropriate dimensions
		X.resize(num_y, num_x);
		Y.resize(num_y, num_x);

		// Populate X: each row is a copy of x_vals
		for (int i = 0; i < num_y; ++i) {
			X.row(i) = x_vals.transpose();
		}

		// Populate Y: each column is a copy of y_vals
		for (int j = 0; j < num_x; ++j) {
			Y.col(j) = y_vals;
		}
	}

	inline int vinq(const std::complex<double>& f) {
		double re = std::real(f);
		double im = std::imag(f);
		if (re > 0 && im >= 0)
			return 1;
		else if (re <= 0 && im > 0)
			return 2;
		else if (re < 0 && im <= 0)
			return 3;
		else if (re >= 0 && im < 0)
			return 4;
		else
			return -1; // NaN equivalent for int
	}

	inline std::vector<CDT::V2d<double>> convertToCDTPoints(const Eigen::ArrayX2d& points) {
		std::vector<CDT::V2d<double>> cdt_points;
		for (int i = 0; i < points.rows(); ++i) {
			cdt_points.emplace_back(points(i, 0), points(i, 1));
		}
		return cdt_points;
	}

	inline std::vector<int> vertexAttachment(const CDT::VertInd& vertInd, const CDT::TriangleVec& elements) {
		std::vector<int> indTriangles;
		for (int i = 1; i < elements.size(); ++i) {
			// check if any vertex of the triangle is in the candidate
			auto tri = elements[i];
			if (std::find(tri.vertices.begin(), tri.vertices.end(), vertInd) != tri.vertices.end()) {
				indTriangles.push_back(i); // Get index of triangle
			}
		}
		return indTriangles;
	}

	inline std::vector<int> edgeAttachment(const CDT::Edge& edge, const CDT::TriangleVec& elements) {
		std::vector<int> indTriangles;
		for (int i = 1; i < elements.size(); ++i) {
			// check if any vertex of the triangle is in the candidate
			auto tri = elements[i];
			if (std::find(tri.vertices.begin(), tri.vertices.end(), edge.v1()) != tri.vertices.end()) {
				if (std::find(tri.vertices.begin(), tri.vertices.end(), edge.v2()) != tri.vertices.end()) {
					indTriangles.push_back(i); // Get index of triangle
				}
			}
		}
		return indTriangles;
	}
}
#endif //GRPFC_UTILS_H
