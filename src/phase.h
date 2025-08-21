//
// Created by Ziang on 2025/8/19.
//

#ifndef GRPFC_PHASE_GRAD_H
#define GRPFC_PHASE_GRAD_H

namespace grpfc {
	int phaseAnalyze(const CDT::EdgeUSet& edges, const std::vector<int>& quadrants,
	                 std::vector<int>& phasesDiff, CDT::EdgeUSet& candidateEdges);
}

#endif //GRPFC_PHASE_GRAD_H
