// SA-GRPF implemented in C++
// Origin Project homepage: https://github.com/PioKow/SAGRPF

#include <iostream>
#include <vector>
#include "Eigen/dense"
#include "CDT.h"
#include "initial_mesh.h"
#include  "utils.h"
#include "params.h"
// #include "adaptive.h"
// #include "analyse_gradients.h"
// #include "analyse_regions.h"
// #include "regular.h" // To be implemented
// #include "rect_dom.h" // To be implemented
// #include "fun.h" // To be implemented
// #include "vinq.h" // To be implemented
// #include "vis.h" // To be implemented

#include "phase_grad.h"
#include "triangulation.h"
#include "../examples/rational_function/analysis_setup.cpp"


int main() {
	// Initialization parameters
	int Mode = 1; // 0: Adaptive, 1: Regular, 2: Aborted, 3: Accuracy achieved
	int ItMax = 1; // Example value, set as needed
	int NodesMin = 10; // Example value
	int NodesMax = 1000; // Example value
	double Tol = 1e-6; // Example value
	double xb = -0.01;
	double xe = 1.0;
	double yb = -100.0;
	double ye = 1.0; // Domain bounds, set as needed
	// Optional parameters for fun()
	double epsilon = 0.01;

	// set up the analysis parameters
	AnalysisParams params{xb, xe, yb, ye, Tol, NodesMin, NodesMax, ItMax, Mode};

	// initialize the variables
	int it = 0;
	size_t nofNodes = 0; // Number of nodes, to be updated during iterations
	Eigen::ArrayX2d nodesCoord(0, 2); // Initial empty nodes coordinate array
	// Generate initial mesh
	Eigen::ArrayX2d newNodesCoord = rect_dom(xb, xe, yb, ye);
	Eigen::ArrayXcd functionValues;
	Eigen::ArrayXi quadrants;
	// PreviousIt struct equivalent
	PreviousIt previousIt;

	while (it < params.ItMax && params.Mode < 2) {
		// Function evaluation
		std::cout << "Iteration: " << it + 1 << std::endl;
		std::cout << "Evaluating the function at new points: " << newNodesCoord.rows() << " nodes" << std::endl;
		std::vector<std::complex<double>> functionValues;
		std::vector<int> quadrants;
		for (auto coord_in: newNodesCoord.rowwise()) {
			std::complex<double> z_in = std::complex<double>(coord_in(0), coord_in(1));
			auto z_out = fun(z_in, epsilon);
			functionValues.push_back(z_out); // Evaluate the function at new nodes
			quadrants.push_back(grpfc::vinq(z_out));
		}
		// Concat NodesCoord
		auto oldNodesNum = nodesCoord.rows();
		nodesCoord.conservativeResize(nodesCoord.rows() + newNodesCoord.rows(), 2);
		nodesCoord.block(oldNodesNum, 0, newNodesCoord.rows(), 2) = newNodesCoord;
		nofNodes = nodesCoord.rows();
		// Meshing operation
		std::cout << "Triangulation and analysis of: " << nofNodes << " nodes" << std::endl;
		CDT::TriangleVec elements;
		CDT::EdgeUSet edges;
		grpfc::triangulate(nodesCoord, elements, edges);

		// for (auto e: elements) {
		// 	std::cout << e.vertices[0] << " " << e.vertices[1] << " " << e.vertices[2] << std::endl;
		// }
		// for (auto e: edges) {
		// 	std::cout << e.v1() << " " << e.v2() << std::endl;
		// }

		// Eigen::Map<Eigen::ArrayXi> dat(quadrants.data(), quadrants.size());
		// std::cout << dat << std::endl;

		// Phase analysis
		std::vector<int> phasesDiff;
		CDT::EdgeUSet candidateEdges;
		grpfc::phaseAnalyze(edges, quadrants, phasesDiff, candidateEdges);

		if (Mode == 0) {
			// Self-adaptive Mesh Generator Mode
			// adaptive(...);
			// PreviousIt.EdgesToSplit = ...;
			// PreviousIt.Elements = ...;
			// PreviousIt.GradeInElements = ...;

			// if (PreviousIt.EdgesToSplit.empty()) Mode = 3;
			// else if (NodesCoord.size() > NodesMin && NodesCoord.size() < NodesMax) {
			//     // Visualization and user prompt (to be implemented)
			//     Mode = 0; // or set by user
			// } else if (NodesCoord.size() >= NodesMax) Mode = 1;
			// if (Mode == 1) {
			//     std::cout << "The mode has been switched to the regular GRPF" << std::endl;
			// }
		}
		if (Mode == 1) {
			// Regular Global complex Roots and Poles Finding algorithm
			// regular(...);
		}

		// Split the edge in half (to be implemented)
		// NewNodesCoord = ...;

		it++;
		std::cout << "Iteration: " << it << " done" << std::endl;
		std::cout << "----------------------------------------------------------------" << std::endl;
	}

	// Final analysis
	if (Mode == 2) {
		std::cout << "Finish after: " << it << " iteration" << std::endl;
	}
	if (Mode == 3) {
		std::cout << "Assumed accuracy is achieved in iteration: " << it << std::endl;
	}

	// analyse_regions(...); // To be implemented

	return 0;
}
