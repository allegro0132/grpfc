//
// Created by Ziang on 2025/8/19.
//

#ifndef GRPFC_TRIANGULATION_H
#define GRPFC_TRIANGULATION_H

#include "utils.h"

namespace grpfc {
	int triangulate(const std::vector<CDT::V2d<double>>& nodesCDT, CDT::TriangleVec& elements, CDT::EdgeUSet& edges);

	CDT::EdgeUSet find_skinny_elements(const CDT::TriangleVec& elements, const Eigen::ArrayX2d& nodesCoord,
	                                   double skinRatio);

	int findNextNode(const Eigen::MatrixX2d& nodesCoord, const CDT::VertInd& prevNode, const CDT::VertInd& refNode,
	                 const std::vector<CDT::VertInd>& tempNodes);
}
#endif //GRPFC_TRIANGULATION_H
