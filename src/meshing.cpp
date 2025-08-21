//
// Created by Ziang on 2025/8/19.
//

#include "meshing.h"
#include "utils.h"
#include <cmath>


Eigen::MatrixXd rect_dom(double xb, double xe, double yb, double ye, double r) {
	double X = xe - xb;
	double Y = ye - yb;
	int n = std::ceil(Y / r + 1);
	Eigen::MatrixXd NodesCoord;

	if (X == Y) {
		double dy = Y / (n - 1);
		int m = std::ceil(X / std::sqrt(r * r - dy * dy / 4) + 1);
		double dx = X / (m - 1);
		Eigen::VectorXd vx = Eigen::VectorXd::LinSpaced(m, xb, xe);
		Eigen::VectorXd vy = Eigen::VectorXd::LinSpaced(n, yb, ye);
		// meshgrid
		Eigen::MatrixXd x(n, m), y(n, m);
		grpfc::meshgrid(vx, vy, x, y);
		// temp vector
		Eigen::VectorXd temp = Eigen::VectorXd::Ones(n);
		temp(n - 1) = 0.0;
		// y adjustment
		for (int j = 0; j < m; ++j) {
			if (j % 2 == 1) {
				// odd column
				y.col(j) += 0.5 * dy * temp;
			}
		}
		// reshape
		Eigen::VectorXd x_flat = Eigen::Map<Eigen::VectorXd>(x.data(), x.size());
		Eigen::VectorXd y_flat = Eigen::Map<Eigen::VectorXd>(y.data(), y.size());
		// tx, ty for extra points
		std::vector<double> tx;
		for (int k = 2; k <= m; k += 2) {
			tx.push_back((k - 1) * dx + xb);
		}
		Eigen::VectorXd _tx = Eigen::Map<Eigen::VectorXd>(tx.data(), tx.size());
		Eigen::VectorXd ty = Eigen::VectorXd::Constant(tx.size(), yb);
		// append extra points
		int total_points = x_flat.size() + tx.size();
		NodesCoord.resize(total_points, 2);
		NodesCoord.block(0, 0, x_flat.size(), 1) = x_flat;
		NodesCoord.block(0, 1, y_flat.size(), 1) = y_flat;
		NodesCoord.block(x_flat.size(), 0, tx.size(), 1) = _tx;
		NodesCoord.block(x_flat.size(), 1, ty.size(), 1) = ty;
	}
	else if (X >= Y) {
		double dy = Y / (n - 1);
		int m = std::ceil(X / std::sqrt(r * r - dy * dy / 4) + 1);
		double dx = X / (m - 1);
		Eigen::VectorXd vx = Eigen::VectorXd::LinSpaced(m, xb, xe);
		Eigen::VectorXd vy = Eigen::VectorXd::LinSpaced(n, yb, ye);
		// meshgrid
		Eigen::MatrixXd x(n, m), y(n, m);
		grpfc::meshgrid(vx, vy, x, y);
		// temp vector
		Eigen::VectorXd temp = Eigen::VectorXd::Ones(n);
		temp(n - 1) = 0.0;
		// y adjustment
		for (int j = 0; j < m; ++j) {
			if (j % 2 == 1) {
				// odd column
				y.col(j) += 0.5 * dy * temp;
			}
		}
		// reshape
		Eigen::VectorXd x_flat = Eigen::Map<Eigen::VectorXd>(x.data(), x.size());
		Eigen::VectorXd y_flat = Eigen::Map<Eigen::VectorXd>(y.data(), y.size());
		// tx, ty for extra points
		std::vector<double> tx;
		if (m % 2 == 1) {
			for (int k = 2; k <= m; k += 2) {
				tx.push_back((k - 1) * dx + xb);
			}
		}
		else {
			for (int k = 2; k <= m - 1; k += 2) {
				tx.push_back((k - 1) * dx + xb);
			}
			tx.push_back(xe);
		}
		Eigen::VectorXd _tx = Eigen::Map<Eigen::VectorXd>(tx.data(), tx.size());
		Eigen::VectorXd ty = Eigen::VectorXd::Constant(tx.size(), yb);
		// append extra points
		int total_points = x_flat.size() + tx.size();
		NodesCoord.resize(total_points, 2);
		NodesCoord.block(0, 0, x_flat.size(), 1) = x_flat;
		NodesCoord.block(0, 1, y_flat.size(), 1) = y_flat;
		NodesCoord.block(x_flat.size(), 0, tx.size(), 1) = _tx;
		NodesCoord.block(x_flat.size(), 1, ty.size(), 1) = ty;
	}
	else {
		int m = n;
		double dx = X / (m - 1);
		n = std::ceil(Y / std::sqrt(r * r - dx * dx / 4) + 1);
		double dy = Y / (n - 1);

		Eigen::VectorXd vx = Eigen::VectorXd::LinSpaced(m, xb, xe);
		Eigen::VectorXd vy = Eigen::VectorXd::LinSpaced(n, yb, ye);

		// meshgrid
		Eigen::MatrixXd x(n, m), y(n, m);
		grpfc::meshgrid(vx, vy, x, y);
		// temp vector
		Eigen::VectorXd temp = Eigen::VectorXd::Ones(m);
		temp(m - 1) = 0.0;
		// x adjustment
		for (int i = 0; i < n; ++i) {
			if (i % 2 == 1) {
				// odd row
				x.row(i) += 0.5 * dx * temp;
			}
		}

		// reshape
		Eigen::VectorXd x_flat = Eigen::Map<Eigen::VectorXd>(x.data(), x.size());
		Eigen::VectorXd y_flat = Eigen::Map<Eigen::VectorXd>(y.data(), y.size());
		// tx, ty for extra points
		std::vector<double> ty;
		if (n % 2 == 1) {
			for (int k = 2; k <= n; k += 2) {
				ty.push_back((k - 1) * dy + yb);
			}
		}
		else {
			for (int k = 2; k <= n - 1; k += 2) {
				ty.push_back((k - 1) * dy + yb);
			}
			ty.push_back(ye);
		}
		Eigen::VectorXd tx = Eigen::VectorXd::Constant(ty.size(), xb);
		Eigen::VectorXd _ty = Eigen::Map<Eigen::VectorXd>(ty.data(), ty.size());
		// append extra points
		int total_points = x_flat.size() + ty.size();
		NodesCoord.resize(total_points, 2);
		NodesCoord.block(0, 0, x_flat.size(), 1) = x_flat;
		NodesCoord.block(0, 1, y_flat.size(), 1) = y_flat;
		NodesCoord.block(x_flat.size(), 0, tx.size(), 1) = tx;
		NodesCoord.block(x_flat.size(), 1, ty.size(), 1) = _ty;
	}
	return NodesCoord;
}
