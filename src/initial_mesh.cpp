//
// Created by Ziang on 2025/8/19.
//

#include "initial_mesh.h"
#include "utils.h"
#include <vector>
#include <cmath>
#include <algorithm>

std::vector<std::vector<double>> rect_dom(double xb, double xe, double yb, double ye) {
    double X = xe - xb;
    double Y = ye - yb;
    int n = 2;
    std::vector<std::vector<double>> NodesCoord;

    if (X == Y) {
        NodesCoord = {
            {xb, yb},
            {xe, yb},
            {xe, ye},
            {xb, ye}
        };
    } else if (X >= Y) {
        double r = Y / (n - 1);
        double dy = Y / (n - 1);
        int m = std::ceil(X / std::sqrt(r * r - dy * dy / 4) + 1);
        double dx = X / (m - 1);

        std::vector<double> vx = grpfc::linspace(xb, xe, m);
        std::vector<double> vy = grpfc::linspace(yb, ye, n);
        // meshgrid
        std::vector<double> x, y;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                x.push_back(vx[j]);
                y.push_back(vy[i]);
            }
        }
        // temp vector
        std::vector<double> temp(n, 1.0);
        temp[n-1] = 0.0;
        // y adjustment
        for (int j = 0; j < m; ++j) {
            double shift = 0.5 * dy * ((1 + std::pow(-1, j + 1)) / 2);
            for (int i = 0; i < n; ++i) {
                y[j*n + i] += shift * temp[i];
            }
        }
        // reshape
        // already in x, y
        // tx, ty for extra points
        std::vector<double> tx, ty;
        if (m % 2 == 1) {
            for (int k = 2; k <= m; k += 2) {
                tx.push_back((k - 1) * dx + xb);
            }
        } else {
            for (int k = 2; k <= m - 1; k += 2) {
                tx.push_back((k - 1) * dx + xb);
            }
            tx.push_back(xe);
        }
        ty.assign(tx.size(), yb);
        // append extra points
        for (size_t i = 0; i < tx.size(); ++i) {
            x.push_back(tx[i]);
            y.push_back(ty[i]);
        }
        // combine x, y
        for (size_t i = 0; i < x.size(); ++i) {
            NodesCoord.push_back({x[i], y[i]});
        }
    } else {
        int m = n;
        double r = X / (m - 1);
        double dx = X / (m - 1);
        n = std::ceil(Y / std::sqrt(r * r - dx * dx / 4) + 1);
        double dy = Y / (n - 1);

        std::vector<double> vx = grpfc::linspace(xb, xe, m);
        std::vector<double> vy = grpfc::linspace(yb, ye, n);
        // meshgrid
        std::vector<double> x, y;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                x.push_back(vx[j]);
                y.push_back(vy[i]);
            }
        }
        // temp vector
        std::vector<double> temp(m, 1.0);
        temp[m-1] = 0.0;
        // x adjustment
        for (int i = 0; i < n; ++i) {
            double shift = 0.5 * dx * ((1 + std::pow(-1, i + 1)) / 2);
            for (int j = 0; j < m; ++j) {
                x[i*m + j] += shift * temp[j];
            }
        }
        // tx, ty for extra points
        std::vector<double> ty, tx;
        if (n % 2 == 1) {
            for (int k = 2; k <= n; k += 2) {
                ty.push_back((k - 1) * dy + yb);
            }
        } else {
            for (int k = 2; k <= n - 1; k += 2) {
                ty.push_back((k - 1) * dy + yb);
            }
            ty.push_back(ye);
        }
        tx.assign(ty.size(), xb);
        // append extra points
        for (size_t i = 0; i < tx.size(); ++i) {
            x.push_back(tx[i]);
            y.push_back(ty[i]);
        }
        // combine x, y
        for (size_t i = 0; i < x.size(); ++i) {
            NodesCoord.push_back({x[i], y[i]});
        }
    }
    return NodesCoord;
}
