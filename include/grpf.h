//
// Created by Ziang on 2025/8/19.
//

#ifndef GRPFC_REGULAR_H
#define GRPFC_REGULAR_H

#include <Eigen/Dense>
#include "CDT.h"

namespace grpfc {
	// Regular Global complex Roots and Poles Finding approach
	struct RegularResult {
		CDT::EdgeUSet EdgesToSplit;
		int Mode;
	};

	CDT::EdgeUSet regularGRPF(const Eigen::ArrayX2d& nodesCoord, double tol,
	                          const CDT::TriangleVec& elements,
	                          CDT::EdgeUSet candidateEdges, int& mode, double skinRatio = 10);

	CDT::EdgeUSet adaptiveMeshGRPF(
		const Eigen::ArrayX2d&nodesCoord, double tol,
		std::vector<std::complex<double>> functionValues,
		const CDT::TriangleVec&elements,
		const CDT::EdgeUSet&edges,
		CDT::EdgeUSet&candidateEdges, double skinRatio = 10
	);
}

#endif //GRPFC_REGULAR_H
