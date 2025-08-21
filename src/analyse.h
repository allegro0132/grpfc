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
		std::vector<std::vector<int>> Regions;
		std::vector<std::complex<double>> zRoot;
		std::vector<double> zRootsMultiplicity;
		std::vector<std::complex<double>> zPoles;
		std::vector<double> zPolesMultiplicity;
	};

	AnalyseRegionsResult analyse_regions(
		const Eigen::MatrixXd& nodesCoord,
		const CDT::TriangleVec& elements,
		const CDT::EdgeUSet& edges,
		const std::vector<int>& quadrants,
		const std::vector<int>& phasesDiff,
		const CDT::EdgeUSet& candidateEdges
	);
}
#endif //GRPFC_ANALYZE_H