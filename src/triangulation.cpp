//
// Created by Ziang on 2025/8/19.
//

#include "triangulation.h"
#include "CDT.h"

namespace grpfc {
	int triangulate(const std::vector<CDT::V2d<double>>& nodesCDT, CDT::TriangleVec& elements, CDT::EdgeUSet& edges) {
		// convert eigen array to cdt vector
		// Triangulation, Elements, Edges
		CDT::Triangulation<double> cdt;
		cdt.insertVertices(nodesCDT);
		// Delaunay triangulation without constraints (triangulated convex-hull)
		cdt.eraseSuperTriangle();
		// cdt.eraseOuterTrianglesAndHoles();
		elements = cdt.triangles;
		edges = CDT::extractEdgesFromTriangles(cdt.triangles);
		return 0;
	}

	CDT::EdgeUSet find_skinny_elements(const CDT::TriangleVec& elements, const Eigen::ArrayX2d& nodesCoord,
	                                   double skinRatio) {
		CDT::EdgeUSet skinnyEdges;
		for (auto tri: elements) {
			// Get the three edges of the triangle
			CDT::EdgeVec edge_vec;
			for (int i = 0; i < 3; ++i) {
				CDT::Edge e(tri.vertices.at(i), tri.vertices.at((i + 1) % 3));
				edge_vec.push_back(e);
			}
			// Compute side lengths
			std::vector<double> sideLength;
			for (auto e: edge_vec) {
				Eigen::Vector2d diff = nodesCoord.row(e.v1()) - nodesCoord.row(e.v2());
				sideLength.push_back(diff.norm());
			}
			// Get triangle vertices
			double xa = nodesCoord(tri.vertices[0], 0);
			double ya = nodesCoord(tri.vertices[0], 1);
			double xb = nodesCoord(tri.vertices[1], 0);
			double yb = nodesCoord(tri.vertices[1], 1);
			double xc = nodesCoord(tri.vertices[2], 0);
			double yc = nodesCoord(tri.vertices[2], 1);
			// Area
			double p = std::abs((xb - xa) * (yc - ya) - (yb - ya) * (xc - xa)) / 2.0;
			// Find max side
			auto res = std::max_element(sideLength.begin(), sideLength.end());
			auto idSideMax = std::distance(sideLength.begin(), res);
			double hmin = 2.0 * p / *res;
			double elementRatio = *res / hmin;
			if (elementRatio > skinRatio) {
				skinnyEdges.insert(edge_vec[idSideMax]);
			}
		}
		return skinnyEdges;
	}

	/*
	 * finds the next node in the candidate region boudary process. The next one (after the reference one) is picked from
	 * the fixed set of nodes.
	 */
	int findNextNode(const Eigen::MatrixX2d& nodesCoord, const CDT::VertInd& prevNode, const CDT::VertInd& refNode,
	                 const std::vector<CDT::VertInd>& tempNodes) {
		auto numTempNodes = tempNodes.size();
		Eigen::Vector2d coordP = nodesCoord.row(prevNode);
		Eigen::Vector2d coordS = nodesCoord.row(refNode);
		Eigen::MatrixX2d coordNs(numTempNodes, 2);
		for (Eigen::Index i = 0; i < numTempNodes; ++i) {
			coordNs.row(i) = nodesCoord.row(tempNodes[i]);
		}
		// broadcast (2, 1) * (1, 2) to  (2, 2)
		Eigen::ArrayX2d sp = (coordP - coordS).transpose().replicate(numTempNodes, 1);

		Eigen::ArrayX2d sn = coordNs.rowwise() - coordS.transpose();

		Eigen::ArrayXd lenSP = sp.rowwise().norm();
		Eigen::ArrayXd lenSN = sn.rowwise().norm();

		Eigen::ArrayXd dotProd = sp.col(0) * sn.col(0) + sp.col(1) * sn.col(1);
		Eigen::ArrayXd phi = (dotProd * (lenSP * lenSN).inverse()).acos();

		for (int i = 0; i < phi.rows(); ++i) {
			if (sp(i, 0) * sn(i, 1) - sp(i, 1) * sn(i, 0) < 0)
				phi(i) = 2 * M_PI - phi(i);
		}

		Eigen::Index minIndex;
		phi.minCoeff(&minIndex);
		return static_cast<int>(minIndex);
	}
}
