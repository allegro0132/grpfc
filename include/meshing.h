//
// Created by Ziang on 2025/8/19.
//

#ifndef GRPFC_INITIAL_MESH_H
#define GRPFC_INITIAL_MESH_H
#include <Eigen/Dense>

// Generates the initial mesh for a rectangular domain
Eigen::MatrixXd rect_dom(double xb, double xe, double yb, double ye, double r);

// Generative the initial mesh for a disk domain
Eigen::MatrixXd disk_dom(double radius, int num_points);

#endif //GRPFC_INITIAL_MESH_H
