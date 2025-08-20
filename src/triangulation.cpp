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
}
