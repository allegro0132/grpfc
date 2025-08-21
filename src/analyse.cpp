//
// Created by Ziang on 2025/8/20.
//

#include "analyse.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>

#include "triangulation.h"
#include "utils.h"

namespace grpfc {
	AnalyseRegionsResult analyse_regions(
		const Eigen::MatrixXd&nodesCoord,
		const CDT::TriangleVec&elements,
		const CDT::EdgeUSet&edges,
		const std::vector<int>&quadrants,
		const std::vector<int>&phasesDiff,
		const CDT::EdgeUSet&candidateEdges
	) {
		AnalyseRegionsResult result;
		int numElements = elements.size();
		std::cout << "Evaluation of regions and verification..." << std::endl;
		if (candidateEdges.size() == 0) {
			std::cout << "No roots in the domain!" << std::endl;
			return result;
		}
		// Evaluation of contour edges from all candidates edges
		std::set<int> idxCandidateElements;
		for (auto edge: candidateEdges) {
			for (int elem: edgeAttachment(edge, elements)) {
				idxCandidateElements.insert(elem);
			}
		}
		// Get candidate elements
		std::set<std::pair<CDT::VertInd, CDT::VertInd>> contourEdges; // must use directed edge
		for (int idx: idxCandidateElements) {
			auto tri = elements[idx];
			// construct contour edges
			for (int i = 0; i < 3; i++) {
				DirectedEdge edge(tri.vertices[i], tri.vertices[(i + 1) % 3]);
				DirectedEdge rEdge(tri.vertices[(i + 1) % 3], tri.vertices[i]);
				if (contourEdges.contains(edge)) {
					contourEdges.erase(edge);
				}
				else if (contourEdges.contains(rEdge)) {
					contourEdges.erase(rEdge);
				}
				else {
					contourEdges.insert(edge);
				}
			}
		}
		std::cout << "Contour edges: " << contourEdges.size() << std::endl;
		// for (const auto& edge: contourEdges) {
		// 	std::cout << edge.first << " " << edge.second << std::endl;
		// }

		// Evaluation of the regions
		// initialize
		std::vector<std::vector<CDT::VertInd>> regions;
		auto tempEdge = *contourEdges.begin();
		contourEdges.erase(tempEdge);
		regions.push_back({tempEdge.first});
		auto refNode = tempEdge.second;
		while (!contourEdges.empty()) {
			auto it = std::find_if(contourEdges.begin(), contourEdges.end(), [&](const DirectedEdge&edge) {
				return edge.first == refNode;
			});
			if (it == contourEdges.end()) {
				regions.back().push_back(refNode);
				if (!contourEdges.empty()) {
					tempEdge = *contourEdges.begin();
					contourEdges.erase(tempEdge);
					regions.push_back({tempEdge.first});
					refNode = tempEdge.second;
				}
			}
			else {
				auto it2 = std::find_if(std::next(it), contourEdges.end(), [&](const DirectedEdge&edge) {
					return edge.first == refNode;
				});
				if (it2 != contourEdges.end()) {
					// If multiple, use find_next_node
					auto prevNode = regions.back().back();
					std::vector<CDT::VertInd> tempNodes{it2->second};
					std::vector<std::set<std::pair<CDT::VertInd, CDT::VertInd>>::iterator> arIt{it2};
					while (it2 != contourEdges.end()) {
						it2 = std::find_if(std::next(it2), contourEdges.end(),
						                   [&](const DirectedEdge&edge) {
							                   return edge.first == refNode;
						                   });
						tempNodes.push_back(it2->second);
						arIt.push_back(it2);
						++it2; // Move to the next element to search for the next occurrence
						auto index = grpfc::findNextNode(nodesCoord, prevNode, refNode, tempNodes);
						std::cout << "Index: " << index << std::endl;
						it = arIt[index];
					}
				}
				regions.back().push_back(it->first);
				refNode = it->second;
				contourEdges.erase(*it);
			}
		}
		regions.back().push_back(refNode);
		// test
		// n=300
		// auto index = grpfc::findNextNode(nodesCoord, 52124, 52125, {51826, 52425});
		// n=600
		// auto index = grpfc::findNextNode(nodesCoord, 208049, 208050, {208649, 207450});
		// std::cout << "Index: " << index << std::endl;

		std::cout << "Region: " << regions.size() << std::endl;
		for (auto region: regions) {
			for (auto v: region) {
				std::cout << v << " ";
			}
			std::cout << "Size: " << region.size() << std::endl;
		}
		// Results
		// std::vector<double> q;
		// std::vector<std::complex<double>> z;
		// for (const auto&region: Regions) {
		// 	std::vector<int> QuadrantSequence;
		// 	for (int idx: region) {
		// 		QuadrantSequence.push_back(Quadrants(idx));
		// 	}
		// 	std::vector<double> dQ;
		// 	for (size_t i = 1; i < QuadrantSequence.size(); ++i) {
		// 		double diff = QuadrantSequence[i] - QuadrantSequence[i - 1];
		// 		if (diff == 3) diff = -1;
		// 		if (diff == -3) diff = 1;
		// 		if (std::abs(diff) == 2) diff = NAN;
		// 		dQ.push_back(diff);
		// 	}
		// 	double qsum = 0.0;
		// 	for (double val: dQ) {
		// 		if (!std::isnan(val)) qsum += val;
		// 	}
		// 	q.push_back(qsum / 4.0);
		// 	std::set<std::complex<double>> uniqueZ;
		// 	for (int idx: region) {
		// 		uniqueZ.insert(std::complex<double>(NodesCoord(idx, 0), NodesCoord(idx, 1)));
		// 	}
		// 	std::complex<double> zmean(0, 0);
		// 	for (const auto&val: uniqueZ) {
		// 		zmean += val;
		// 	}
		// 	if (!uniqueZ.empty()) zmean /= static_cast<double>(uniqueZ.size());
		// 	z.push_back(zmean);
		// 	// Visualization omitted
		// 	std::cout << "Region: " << (&region - &Regions[0]) + 1 << " z = " << zmean << " with q = " << qsum / 4.0 <<
		// 			std::endl;
		// }
		// result.Regions = Regions;
		// for (size_t i = 0; i < q.size(); ++i) {
		// 	if (q[i] > 0) {
		// 		result.zRoot.push_back(z[i]);
		// 		result.zRootsMultiplicity.push_back(q[i]);
		// 	}
		// }
		// std::cout << "---------------------" << std::endl;
		// std::cout << "Root and its multiplicity: " << std::endl;
		// for (size_t i = 0; i < result.zRoot.size(); ++i) {
		// 	std::cout << result.zRoot[i] << " " << result.zRootsMultiplicity[i] << std::endl;
		// }
		// for (size_t i = 0; i < q.size(); ++i) {
		// 	if (q[i] < 0) {
		// 		result.zPoles.push_back(z[i]);
		// 		result.zPolesMultiplicity.push_back(q[i]);
		// 	}
		// }
		// std::cout << "---------------------" << std::endl;
		// std::cout << "Poles and its multiplicity: " << std::endl;
		// for (size_t i = 0; i < result.zPoles.size(); ++i) {
		// 	std::cout << result.zPoles[i] << " " << result.zPolesMultiplicity[i] << std::endl;
		// }
		// Visualization omitted
		return result;
	}
}
