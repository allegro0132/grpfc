//
// Created by Ziang on 2025/8/20.
//

#include "analyse.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <map>

#include "triangulation.h"
#include "meshing.h"
#include "phase.h"
#include "grpf.h"
#include "utils.h"

GRPFAnalyse::GRPFAnalyse(const AnalyseParams&params,
                         std::string meshType, bool log) {
	this->params = params;
	this->log = log;
	// initialize parameters
	it = 0;
	nodesCoord = Eigen::ArrayX2d::Zero(0, 2); // Initial empty nodes coordinate array
	// start as regular method
	mode = 1; // 0: Adaptive, 1: Regular, 2: Aborted, 3: Accuracy achieved
	// Generate initial mesh
	if (meshType == "disk")
		GenerateDiskMesh();
	else
		GenerateRectangleMesh();
}

GRPFAnalyse::GRPFAnalyse(std::function<std::complex<double>(std::complex<double>)> func, const AnalyseParams&params,
                         std::string meshType, bool log) {
	this->func = func;
	this->params = params;
	this->log = log;
	// initialize parameters
	it = 0;
	nodesCoord = Eigen::ArrayX2d::Zero(0, 2); // Initial empty nodes coordinate array
	// start as regular method
	mode = 1; // 0: Adaptive, 1: Regular, 2: Aborted, 3: Accuracy achieved
	// Generate initial mesh
	if (meshType == "disk")
		GenerateDiskMesh();
	else
		GenerateRectangleMesh();
}

int GRPFAnalyse::GenerateRectangleMesh() {
	// assert params
	assert(params.xb < params.xe && params.yb < params.ye && params.r > 0);
	newNodesCoord = rect_dom(params.xb, params.xe, params.yb, params.ye, params.r);
	return 0;
}

int GRPFAnalyse::GenerateDiskMesh() {
	return 0;
}

int GRPFAnalyse::SplitEdge() {
	newNodesCoord = Eigen::ArrayX2d::Zero(edgesToSplit.size(), 2);
	int i = 0;
	if (edgesToSplit.size() > 0) {
		for (auto e: edgesToSplit) {
			newNodesCoord.row(i) = (nodesCoord.row(e.v1()) + nodesCoord.row(e.v2())) / 2;
			i++;
		}
	}
	return 0;
}

int GRPFAnalyse::EvaluateFunction() {
	if (log)
		std::cout << "Evaluating the function at new points: " << newNodesCoord.rows() << " nodes" << std::endl;
	for (auto coord_in: newNodesCoord.rowwise()) {
		auto z_in = std::complex<double>(coord_in(0), coord_in(1));
		auto z_out = func(z_in);
		functionValues.push_back(z_out); // Evaluate the function at new nodes
		quadrants.push_back(grpfc::vinq(z_out));
	}
	return 0;
}

int GRPFAnalyse::EvaluateFunction(const Eigen::ArrayXcd&newFunctionValues) {
	if (log)
		std::cout << "Evaluating the function at new points: " << newNodesCoord.rows() << " nodes" << std::endl;
	if (newFunctionValues.rows() != newNodesCoord.rows()) {
		std::cerr << "Error: The size of newFunctionValues does not match the number of new nodes." << std::endl;
		return -1;
	}
	for (Eigen::Index i = 0; i < newFunctionValues.rows(); ++i) {
		functionValues.push_back(newFunctionValues(i));
		quadrants.push_back(grpfc::vinq(newFunctionValues(i)));
	}
	return 0;
}

int GRPFAnalyse::Triangulate() {
	if (log)
		std::cout << "Triangulation and analysis of: " << numNodes() << " nodes" << std::endl;
	auto nodesCDT = grpfc::convertToCDTPoints(nodesCoord);
	grpfc::triangulate(nodesCDT, elements, edges);
	return 0;
}

int GRPFAnalyse::RegularGRPF() {
	// Regular Global complex Roots and Poles Finding algorithm
	assert(params.Tol > 0);
	edgesToSplit = grpfc::regularGRPF(nodesCoord, params.Tol, elements, candidateEdges, mode);
	return 0;
}

int GRPFAnalyse::AdaptiveMeshGRPF() {
	// Self-adaptive Mesh Generator Mode
	// adaptive(...);
	// PreviousIt.EdgesToSplit = ...;
	// PreviousIt.Elements = ...;
	// PreviousIt.GradeInElements = ...;

	// if (PreviousIt.EdgesToSplit.empty()) Mode = 3;
	// else if (NodesCoord.size() > NodesMin && NodesCoord.size() < NodesMax) {
	//     // Visualization and user prompt (to be implemented)
	//     Mode = 0; // or set by user
	// } else if (NodesCoord.size() >= NodesMax) Mode = 1;
	// if (Mode == 1) {
	//     std::cout << "The mode has been switched to the regular GRPF" << std::endl;
	// }
	return 0;
}

int GRPFAnalyse::PhaseAnalyse() {
	grpfc::phaseAnalyze(edges, quadrants, phasesDiff, candidateEdges);
	return 0;
}


int GRPFAnalyse::SelfAdaptiveRun() {
	assert(params.ItMax > 0);
	while (it < params.ItMax && mode < 2) {
		// Function evaluation
		EvaluateFunction();
		// Concat NodesCoord
		auto oldNodesNum = nodesCoord.rows();
		nodesCoord.conservativeResize(nodesCoord.rows() + newNodesCoord.rows(), 2);
		nodesCoord.block(oldNodesNum, 0, newNodesCoord.rows(), 2) = newNodesCoord;
		// Meshing operation
		Triangulate();
		// Phase analysis
		PhaseAnalyse();
		if (mode == 0) {
			AdaptiveMeshGRPF();
		}
		else if (mode == 1) {
			RegularGRPF();
		}

		// Split the edge in half
		SplitEdge();

		it++;
		if (log) {
			std::cout << "Iteration: " << it << " done" << std::endl;
			std::cout << "----------------------------------------------------------------" << std::endl;
		}
	}

	// Final analysis
	if (log) {
		if (mode == 2) {
			std::cout << "Finish after: " << it << " iteration" << std::endl;
		}
		else if (mode == 3) {
			std::cout << "Assumed accuracy is achieved in iteration: " << it << std::endl;
		}
	}
	AnalyseRegion();
	// Get Result
	GetRootsAndPoles();
	return 0;
}

int GRPFAnalyse::AnalyseRegion() {
	if (log) {
		std::cout << "Evaluation of regions and verification..." << std::endl;
	}
	if (candidateEdges.size() == 0) {
		std::cout << "No roots in the domain!" << std::endl;
		return 0;
	}
	// Evaluation of contour edges from all candidates edges
	std::set<int> idxCandidateElements;
	for (auto edge: candidateEdges) {
		for (int elem: grpfc::edgeAttachment(edge, elements)) {
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

	// Evaluation of the regions
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
				std::vector tempNodes{it->second};
				std::vector arIt{it};
				while (it2 != contourEdges.end()) {
					tempNodes.push_back(it2->second);
					arIt.push_back(it2);
					it2 = std::find_if(std::next(it2), contourEdges.end(),
					                   [&](const DirectedEdge&edge) {
						                   return edge.first == refNode;
					                   });
				}
				auto index = grpfc::findNextNode(nodesCoord, prevNode, refNode, tempNodes);
				it = arIt[index];
			}
			regions.back().push_back(it->first);
			refNode = it->second;
			contourEdges.erase(*it);
		}
	}
	regions.back().push_back(refNode);
	return 0;
}

AnalyseRegionsResult GRPFAnalyse::GetRootsAndPoles() {
	// Get Results
	std::vector<double> zRootsMultiplicity;
	std::vector<std::complex<double>> zRoots;
	std::vector<double> zPolesMultiplicity;
	std::vector<std::complex<double>> zPoles;
	// Map to eigen array
	Eigen::Map<Eigen::ArrayXi> quadrantsEigen(quadrants.data(), quadrants.size());
	for (const auto&region: regions) {
		auto quadrantSequence = quadrantsEigen(region);
		auto size_region = region.size();
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
		std::vector idxUnique{0};
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
		if (log) {
			std::cout << "Region: --------------------------------------" << std::endl;
			std::cout << "z = " << zEle << std::endl;
			std::cout << "q = " << qEle << std::endl;
		}
	}

	// save data
	result.regions = regions;
	result.zRoots = zRoots;
	result.zRootsMultiplicity = zRootsMultiplicity;
	result.zPoles = zPoles;
	result.zPolesMultiplicity = zPolesMultiplicity;
	if (log) {
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
	}
	// Visualization omitted
	return result;
}
