//
// Created by Ziang on 2025/8/19.
//

#ifndef GRPFC_PARAMS_H
#define GRPFC_PARAMS_H

// define struct contains all parameters of the analysis
// optional,xb,xe,yb,ye,Tol,NodesMin,NodesMax,ItMax,Mode
struct analysisParams {
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
	int NodesMax; // maximum number of nodes
	// Maximum number of iterations (buffer)
	int ItMax; // maximum number of iterations
	// mode of operation (0: Self-adaptive Mesh Generator, 1: Regular Global complex Roots and Poles Finding algorithm)
	int Mode;
};
#endif //GRPFC_PARAMS_H
