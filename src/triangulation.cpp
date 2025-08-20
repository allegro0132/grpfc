//
// Created by Ziang on 2025/8/19.
//

#include "triangulation.h"
#include "CDT.h"

namespace grpfc {
	int triangulate(const Eigen::ArrayX2d& nodesCoord, CDT::TriangleVec& elements, CDT::EdgeUSet& edges) {
		// convert eigen array to cdt vector
		auto nodesCDT = convertToCDTPoints(nodesCoord);
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
}
