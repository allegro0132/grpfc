//
// Created by Ziang on 2025/8/19.
//

#include "regular.h"
#include "CDT.h"
#include "regular.h"
#include "utils.h"
#include <Eigen/Dense>
#include <iostream>
#include <cmath>
#include <set>

#include "triangulation.h"

namespace grpfc {
	int regularGRPF(const Eigen::ArrayX2d& nodesCoord, double tol,
	                const CDT::TriangleVec& elements,
	                CDT::EdgeUSet& candidateEdges, int& mode) {
		mode = 1;
		if (candidateEdges.empty()) {
			mode = 2;
			return 0;
		}
		// Calculate CandidateEdgesLengths
		std::vector<double> candidateEdgesLengths;
		for (auto e: candidateEdges) {
			Eigen::Vector2d diff = nodesCoord.row(e.v1()) - nodesCoord.row(e.v2());
			auto norm = diff.norm();
			candidateEdgesLengths.push_back(norm);
		}
		double minCandidateEdgesLengths = *std::min_element(candidateEdgesLengths.begin(), candidateEdgesLengths.end());
		double maxCandidateEdgesLengths = *std::max_element(candidateEdgesLengths.begin(), candidateEdgesLengths.end());
		std::cout << "Candidate edges length min: " << minCandidateEdgesLengths
				<< " max: " << maxCandidateEdgesLengths << std::endl;
		if (maxCandidateEdgesLengths < tol) {
			mode = 3;
		}
		// construct the CDT::VertInd set for candidate edges
		std::set<CDT::VertInd> uniqueVert;
		for (auto e: candidateEdges) {
			uniqueVert.insert(e.v1());
			uniqueVert.insert(e.v2());
		}
		// auto indTriangles = grpfc::vertexAttachment(uniqueVert, elements);
		std::vector<std::vector<int>> arCandidateElements;
		for (auto ind: uniqueVert) {
			auto indTriangles = grpfc::vertexAttachment(ind, elements);
			arCandidateElements.push_back(indTriangles);
		}
		// for (auto ind: indTriangles) {
		// 	std::cout << ind << std::endl;
		// 	for (auto v: elements[ind].vertices) {
		// 		std::cout << nodesCoord.row(v) << " ";
		// 	}
		// 	std::cout << std::endl;
		// }

		std::vector<int> numConnectionsToCandidate(elements.size(), 0);
		for (auto indTriangles: arCandidateElements) {
			for (auto idx: indTriangles) {
				numConnectionsToCandidate[idx]++;
			}
		}

		CDT::TriangleVec candidateElements;
		for (int i = 0; i < elements.size(); i++) {
			if (numConnectionsToCandidate[i] > 0) {
				candidateElements.push_back(elements[i]);
			}
		}

		auto skinnyEdges = grpfc::find_skinny_elements(candidateElements, nodesCoord, 10);

		// concat candidateEdges and skinnyEdges
		for (auto skinny: skinnyEdges) {
			candidateEdges.insert(skinny);
		}

		// filter candidateEdges based on tol
		for (auto it = candidateEdges.begin(); it != candidateEdges.end();) {
			auto edge = *it;
			Eigen::Vector2d diff = nodesCoord.row(edge.v1()) - nodesCoord.row(edge.v2());
			if (diff.norm() <= tol)
				// drop the edge less/equal to tol
				it = candidateEdges.erase(it);
			else
				++it;
		}
		return 0;
	}

}
