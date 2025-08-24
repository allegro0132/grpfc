//
// Created by Ziang on 2025/8/19.
//

#include "grpf.h"
#include "CDT.h"
#include "utils.h"
#include <Eigen/Dense>
#include <iostream>
#include <set>

#include "triangulation.h"

namespace grpfc {
	CDT::EdgeUSet regularGRPF(const Eigen::ArrayX2d& nodesCoord, double tol,
	                          const CDT::TriangleVec& elements,
	                          CDT::EdgeUSet candidateEdges, int& mode, double skinRatio) {
		mode = 1;
		if (candidateEdges.empty()) {
			mode = 2;
			return {};
		}
		// Calculate CandidateEdgesLengths
		std::vector<double> candidateEdgesLengths;
		for (auto e: candidateEdges) {
			Eigen::Vector2d diff = nodesCoord.row(e.v1()) - nodesCoord.row(e.v2());
			auto norm = diff.norm();
			candidateEdgesLengths.push_back(norm);
		}
		// double minCandidateEdgesLengths = *std::min_element(candidateEdgesLengths.begin(), candidateEdgesLengths.end());
		double maxCandidateEdgesLengths = *std::max_element(candidateEdgesLengths.begin(), candidateEdgesLengths.end());
		// std::cout << "Candidate edges length min: " << minCandidateEdgesLengths
		// 		<< " max: " << maxCandidateEdgesLengths << std::endl;
		if (maxCandidateEdgesLengths < tol) {
			mode = 3;
		}
		// construct the CDT::VertInd set for candidate edges
		std::set<CDT::VertInd> uniqueVert;
		for (auto e: candidateEdges) {
			uniqueVert.insert(e.v1());
			uniqueVert.insert(e.v2());
		}

		std::vector<std::vector<int>> arCandidateElements;
		for (auto ind: uniqueVert) {
			auto indTriangles = vertexAttachment(ind, elements);
			arCandidateElements.push_back(indTriangles);
		}

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

		auto skinnyEdges = find_skinny_elements(candidateElements, nodesCoord, skinRatio);

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
		return candidateEdges;
	}

	CDT::EdgeUSet adaptiveMeshGRPF(
		const Eigen::ArrayX2d&nodesCoord, double tol,
		const std::vector<std::complex<double>>&functionValues,
		CDT::TriangleVec&elements,
		const CDT::EdgeUSet&edges,
		CDT::EdgeUSet&candidateEdges, double skinRatio) {
		// // Edge lengths
		// Eigen::VectorXd EdgesLengths(Edges.rows());
		// for (int i = 0; i < Edges.rows(); ++i)
		// 	EdgesLengths(i) = (NodesCoord.row(Edges(i, 1)) - NodesCoord.row(Edges(i, 0))).norm();
		// double TolAdaptive = EdgesLengths.minCoeff();
		// std::cout << "Edges length min: " << TolAdaptive << " max: " << EdgesLengths.maxCoeff() << std::endl;
		// if (TolAdaptive <= TolGlobal)
		// 	TolAdaptive = TolGlobal;
		//
		// // New elements
		// Eigen::MatrixXi ElementsPrevious = PreviousIt.Elements;
		// std::vector<bool> NewElementsBool(Elements.rows(), true);
		// std::vector<int> NewElementsId(Elements.rows(), -1);
		// if (ElementsPrevious.size() != 0) {
		// 	// Compare sorted rows
		// 	// ...implement row sorting and comparison...
		// }
		// // I - skiny
		// Eigen::MatrixXi SkinyEdges = find_skiny_elements(Elements, NodesCoord, 10);
		//
		// // II - phase extremum
		// Eigen::MatrixXi EdgesToSplitPrevious = PreviousIt.EdgesToSplit;
		// int NrOfNodes = NodesCoord.rows() - EdgesToSplitPrevious.rows();
		// std::vector<int> PhaseFlagOnEdges(EdgesToSplitPrevious.rows(), 1);
		// if (EdgesToSplitPrevious.size() != 0) {
		// 	for (int ik = 0; ik < EdgesToSplitPrevious.rows(); ++ik) {
		// 		PhaseFlagOnEdges[ik] = phase_validation(
		// 			FunctionValues(EdgesToSplitPrevious(ik, 0)),
		// 			FunctionValues(EdgesToSplitPrevious(ik, 1)),
		// 			FunctionValues(ik + NrOfNodes)
		// 		);
		// 	}
		// }
		// std::vector<int> ExtremeNodesId;
		// for (int i = 0; i < PhaseFlagOnEdges.size(); ++i) {
		// 	if (PhaseFlagOnEdges[i] == 2 || std::isnan(PhaseFlagOnEdges[i]))
		// 		ExtremeNodesId.push_back(NrOfNodes + i);
		// }
		// Eigen::MatrixXi ExtremeEdges = get_edges_attach_toVertix(DT, Elements, ExtremeNodesId);
		//
		// // Cumulate edges
		// std::vector<Eigen::MatrixXi> edgeSets = {CandidateEdges, ExtremeEdges, SkinyEdges};
		// int totalRows = 0;
		// for (const auto&mat: edgeSets) totalRows += mat.rows();
		// Eigen::MatrixXi EdgesToSplit(totalRows, 2);
		// int row = 0;
		// for (const auto&mat: edgeSets) {
		// 	EdgesToSplit.block(row, 0, mat.rows(), 2) = mat;
		// 	row += mat.rows();
		// }
		// // Unique rows
		// // ...implement unique row logic...
		// // Filter by TolGlobal
		// // ...implement filter logic...
		//
		// // III - gradienty
		// Eigen::MatrixXd GradeInElements = Eigen::MatrixXd::Zero(Elements.rows(), 2);
		// if (PreviousIt.GradeInElements.size() != 0) {
		// 	// ...copy previous gradients for old elements...
		// }
		// std::vector<bool> GradeElementsBool(Elements.rows(), true);
		// for (int ik = 0; ik < CandidateEdges.rows(); ++ik) {
		// 	// ...implement logic for GradeElementsBool...
		// }
		// // Calculate new gradients
		// // ...implement gradient calculation...
		//
		// // Determine most important edge
		// // ...implement TopEdge logic...
		//
		// // Final return
		// // ...append TopEdge to EdgesToSplit...
		//
		// result.EdgesToSplit = EdgesToSplit;
		// result.GradeInElements = GradeInElements;
		// return result;
		return {};
	}
}
