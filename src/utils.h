//
// Created by Ziang on 2025/8/19.
//

#ifndef GRPFC_UTILS_H
#define GRPFC_UTILS_H

#pragma once
#include <vector>
#include <numeric>
#include <complex>
#include <fstream>
#include "../include/gnuplot.h"

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

	std::string getPhaseData(const std::function<std::complex<double>(std::complex<double>)>&fun) {
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

	std::string getMagnitudeData(const std::function<std::complex<double>(std::complex<double>)>&fun) {
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

	template <typename T>
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
}
#endif //GRPFC_UTILS_H
