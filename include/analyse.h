//
// Created by Ziang on 2025/8/20.
//

#ifndef GRPFC_ANALYZE_H
#define GRPFC_ANALYZE_H
#include <vector>

// Analyse mesh regions, roots, and poles
#include <complex>
#include "CDT.h"
#include "utils.h"

using DirectedEdge = std::pair<CDT::VertInd, CDT::VertInd>;

// define struct contains all parameters of the analysis
// optional,xb,xe,yb,ye,Tol,NodesMin,NodesMax,ItMax,Mode
struct AnalyseParams {
	double r; // initial mesh step
	double xb; // real part begin
	double xe; // real part end
	double yb; // imaginary part begin
	double ye; // imaginary part end
	double Tol; // tolerance for finding roots
	// the number of points below which the adaptive mode is automatically used (without interrupt possibilities)
	// set 0 if you want to manually choose the mode after each iteration
	int NodesMin; // minimum number of nodes
	// the number of points after that the regular mode is automatically switched (without interrupt possibilities)
	// set Inf if you want to manually choose the mode after each iteration
	double NodesMax; // maximum number of nodes
	// Maximum number of iterations (buffer)
	int ItMax; // maximum number of iterations
};

struct AnalyseRegionsResult {
	std::vector<std::vector<CDT::VertInd>> regions;
	std::vector<std::complex<double>> zRoots;
	std::vector<double> zRootsMultiplicity;
	std::vector<std::complex<double>> zPoles;
	std::vector<double> zPolesMultiplicity;
};

class GRPFAnalyse {
public:
	AnalyseParams params;
	std::function<std::complex<double>(std::complex<double>)> func;
	int it;
	Eigen::ArrayX2d nodesCoord; // Initial empty nodes coordinate array
	Eigen::ArrayX2d newNodesCoord; // New nodes to be added at each iteration
	CDT::TriangleVec elements;
	CDT::EdgeUSet edges;
	std::vector<int> phasesDiff;
	std::vector<int> quadrants;
	AnalyseRegionsResult result;
	std::vector<std::vector<CDT::VertInd>> regions;
	// mode of operation (0: Self-adaptive Mesh Generator, 1: Regular Global complex Roots and Poles Finding algorithm,
	// 2: Aborted, 3: Accuracy achieved)
	int mode;
	std::vector<std::complex<double>> functionValues;

	GRPFAnalyse(const AnalyseParams&params,
	            std::string meshType = "rect", bool log = false);

	GRPFAnalyse(std::function<std::complex<double>(std::complex<double>)> func, const AnalyseParams&params,
	            std::string meshType = "rect", bool log = false);

	Eigen::Index numNodes() { return nodesCoord.rows(); }

	int GenerateRectangleMesh();

	int GenerateDiskMesh();

	int SelfAdaptiveRun();

	int EvaluateFunction();

	int EvaluateFunction(const Eigen::ArrayXcd&newFunctionValues);

	int RegularGRPF();

	int AdaptiveMeshGRPF();

	int Triangulate();

	int SplitEdge();

	int PhaseAnalyse();

	int AnalyseRegion();

	AnalyseRegionsResult GetRootsAndPoles();

private:
	bool log;
	CDT::EdgeUSet candidateEdges;
	CDT::EdgeUSet edgesToSplit;
};

namespace grpfc {
	AnalyseRegionsResult analyseRegions(
		const Eigen::MatrixXd&nodesCoord,
		const CDT::TriangleVec&elements,
		std::vector<int>&quadrants,
		const CDT::EdgeUSet&candidateEdges
	);
}
#endif //GRPFC_ANALYZE_H
