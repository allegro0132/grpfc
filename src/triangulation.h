//
// Created by Ziang on 2025/8/19.
//

#ifndef GRPFC_TRIANGULATION_H
#define GRPFC_TRIANGULATION_H

#include "utils.h"

namespace grpfc {
	int triangulate(const Eigen::ArrayX2d& nodesCoord, CDT::TriangleVec& elements, CDT::EdgeUSet& edges);
}
#endif //GRPFC_TRIANGULATION_H