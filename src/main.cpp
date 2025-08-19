// SA-GRPF implemented in C++
// Origin Project homepage: https://github.com/PioKow/SAGRPF

#include <iostream>
#include <vector>
#include "adaptive.h"
#include "analyse_gradients.h"
#include "analyse_regions.h"
// #include "regular.h" // To be implemented
// #include "rect_dom.h" // To be implemented
// #include "fun.h" // To be implemented
// #include "vinq.h" // To be implemented
// #include "vis.h" // To be implemented

int main() {
    // Initialization
    int it = 0;
    int Mode = 0; // 0: Adaptive, 1: Regular, 2: Aborted, 3: Accuracy achieved
    int ItMax = 100; // Example value, set as needed
    int NodesMin = 10; // Example value
    int NodesMax = 1000; // Example value
    double Tol = 1e-6; // Example value
    // double xb, xe, yb, ye; // Domain bounds, set as needed
    // Optional parameters for fun()
    // auto Optional = ...;

    std::vector<std::vector<double>> NodesCoord;
    std::vector<std::vector<double>> NewNodesCoord;
    std::vector<double> FunctionValues;
    std::vector<int> Quadrants;
    // PreviousIt struct equivalent
    struct PreviousItType {
        std::vector<std::vector<int>> EdgesToSplit;
        std::vector<std::vector<int>> Elements;
        std::vector<double> GradeInElements;
    } PreviousIt;

    // Load analysis parameters (to be implemented)
    // analysis_parameters();

    // Generate initial mesh (to be implemented)
    // NewNodesCoord = rect_dom(xb, xe, yb, ye);

    while (it < ItMax && Mode < 2) {
        // Function evaluation
        for (size_t node = NodesCoord.size(); node < NodesCoord.size() + NewNodesCoord.size(); ++node) {
            // FunctionValues[node] = fun(NodesCoord[node], Optional);
            // Quadrants[node] = vinq(FunctionValues[node]);
        }
        double SingleNodeTime = 0.0; // To be computed

        // Meshing operation (to be implemented)
        // Triangulation, Elements, Edges
        // std::vector<std::vector<int>> Elements;
        // std::vector<std::vector<int>> Edges;

        // Phase analysis (to be implemented)
        // std::vector<double> PhasesDiff;
        // std::vector<std::vector<int>> CandidateEdges;

        if (Mode == 0) {
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
        }
        if (Mode == 1) {
            // Regular Mode
            // regular(...);
        }

        // Split the edge in half (to be implemented)
        // NewNodesCoord = ...;

        it++;
        std::cout << "Iteration: " << it << " done" << std::endl;
        std::cout << "----------------------------------------------------------------" << std::endl;
    }

    // Final analysis
    if (Mode == 2) {
        std::cout << "Finish after: " << it << " iteration" << std::endl;
    }
    if (Mode == 3) {
        std::cout << "Assumed accuracy is achieved in iteration: " << it << std::endl;
    }

    // analyse_regions(...); // To be implemented

    return 0;
}
