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
		const Eigen::MatrixXd& nodesCoord,
		const CDT::TriangleVec& elements,
		std::vector<int>& quadrants,
		const CDT::EdgeUSet& candidateEdges
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

		// std::cout << "Region: " << regions.size() << std::endl;
		// for (auto region: regions) {
		// 	for (auto v: region) {
		// 		std::cout << v << " ";
		// 	}
		// 	std::cout << "Size: " << region.size() << std::endl;
		// }

		// Results
		std::vector<double> zRootsMultiplicity;
		std::vector<std::complex<double>> zRoots;
		std::vector<double> zPolesMultiplicity;
		std::vector<std::complex<double>> zPoles;
		// Map to eigen array
		Eigen::Map<Eigen::ArrayXi> quadrantsEigen(quadrants.data(), quadrants.size());
		for (const auto&region: regions) {
			auto quadrantSequence = quadrantsEigen(region);
			auto size_region = quadrantSequence.size();
			Eigen::ArrayXi dQ = quadrantSequence.tail(size_region - 1) - quadrantSequence.head(size_region - 1);
			// modify dQ element
			for (int i = 0; i < dQ.size(); i++) {
				if (dQ[i] == 3) dQ[i] = -1;
				else if (dQ[i] == -3) dQ[i] = 1;
				else if (std::abs(dQ[i]) == 2) dQ[i] = -1;
			}
			float qEle = dQ.sum() / 4.0;
			auto nodesRegionReal = nodesCoord.col(0)(region);
			auto nodesRegionImag = nodesCoord.col(1)(region);
			Eigen::ArrayXcd nodesRegionZPlane = nodesRegionReal.cast<std::complex<double>>() + std::complex<double>(0, 1) *
			                                    nodesRegionImag.cast<std::complex<double>>();
			// remove duplicated z points
			std::sort(nodesRegionZPlane.begin(), nodesRegionZPlane.end(),
			          [](const std::complex<double>&a, const std::complex<double>&b) {
				          if (a.real() != b.real()) {
					          return a.real() < b.real();
				          }
				          return a.imag() < b.imag();
			          });

			// Find unique elements
			std::vector<int> idxUnique{0};
			for (int i = 1; i < nodesRegionZPlane.size(); i++) {
				if (nodesRegionZPlane[i] != nodesRegionZPlane(i - 1)) {
					idxUnique.push_back(i);
				}
			}
			auto zEle = nodesRegionZPlane(idxUnique).mean();
			if (qEle > 0) {
				zRoots.push_back(zEle);
				zRootsMultiplicity.push_back(qEle);
			}
			else if (qEle < 0) {
				zPoles.push_back(zEle);
				zPolesMultiplicity.push_back(qEle);
			}

			std::cout << "Region: --------------------------------------" << std::endl;
			std::cout << "z = " << zEle << std::endl;
			std::cout << "q = " << qEle << std::endl;
		}

		// save data
		result.regions = regions;
		result.zRoots = zRoots;
		result.zRootsMultiplicity = zRootsMultiplicity;
		result.zPoles = zPoles;
		result.zPolesMultiplicity = zPolesMultiplicity;

		std::cout << "---------------------" << std::endl;
		std::cout << "Root and its multiplicity: " << std::endl;
		for (size_t i = 0; i < result.zRoots.size(); ++i) {
			std::cout << result.zRoots[i] << " " << result.zRootsMultiplicity[i] << std::endl;
		}

		std::cout << "---------------------" << std::endl;
		std::cout << "Poles and its multiplicity: " << std::endl;
		for (size_t i = 0; i < result.zPoles.size(); ++i) {
			std::cout << result.zPoles[i] << " " << result.zPolesMultiplicity[i] << std::endl;
		}
		// Visualization omitted
		return result;
	}
}
