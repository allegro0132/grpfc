//
// Created by Ziang on 2025/8/19.
//
#include "CDT.h"
#include "phase_grad.h"

namespace grpfc {
	int phaseAnalyze(const CDT::EdgeUSet& edges, const std::vector<int>& quadrants,
	                 std::vector<int>& phasesDiff, CDT::EdgeUSet& candidateEdges) {
		for (auto e: edges) {
			int phaseDiff = std::abs(quadrants.at(e.v1()) - quadrants.at(e.v2()));
			if (phaseDiff == 3) { phaseDiff = 1; }
			phasesDiff.push_back(phaseDiff);
			if (phaseDiff == 2 or phaseDiff == -1) { candidateEdges.insert(e); }
		}
		return 0;
	}
}
