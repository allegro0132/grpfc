//
// Created by Ziang on 2025/8/20.
//

#ifndef GRPFC_ANALYZE_H
#define GRPFC_ANALYZE_H
#include <vector>

// Analyse mesh regions, roots, and poles
#include <Eigen/Dense>
#include <complex>
#include "CDT.h"

using DirectedEdge = std::pair<CDT::VertInd, CDT::VertInd>;

// Placeholder for DT type
class DTType;

namespace grpfc {
	struct AnalyseRegionsResult {
		std::vector<std::vector<CDT::VertInd>> regions;
		std::vector<std::complex<double>> zRoots;
		std::vector<double> zRootsMultiplicity;
		std::vector<std::complex<double>> zPoles;
		std::vector<double> zPolesMultiplicity;
	};

	AnalyseRegionsResult analyse_regions(
		const Eigen::MatrixXd& nodesCoord,
		const CDT::TriangleVec& elements,
		std::vector<int>& quadrants,
		const CDT::EdgeUSet& candidateEdges
	);
}
#endif //GRPFC_ANALYZE_H