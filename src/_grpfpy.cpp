//
// Created by Ziang on 2025/8/24.
//
#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/string.h>
// #include <nanobind/stl/unordered_set.h>
#include <nanobind/stl/function.h>
#include <nanobind/eigen/dense.h>
#include "analyse.h"

namespace nb = nanobind;
using namespace nb::literals;

NB_MODULE(_grpfpy, m) {
	// Binding Analysis parameters struct
	nb::class_<AnalyseParams>(m, "AnalyseParams")
			.def(nb::init<double, double, double, double, double, double, int, double, int>())
			.def_rw("r", &AnalyseParams::r)
			.def_rw("xb", &AnalyseParams::xb)
			.def_rw("xe", &AnalyseParams::xe)
			.def_rw("yb", &AnalyseParams::yb)
			.def_rw("ye", &AnalyseParams::ye)
			.def_rw("tol", &AnalyseParams::Tol)
			.def_rw("nodes_min", &AnalyseParams::NodesMin)
			.def_rw("nodes_max", &AnalyseParams::NodesMax)
			.def_rw("it_max", &AnalyseParams::ItMax);

	// Binding AnalyseRegionsResult struct
	nb::class_<AnalyseRegionsResult>(m, "AnalyseRegionsResult")
			.def(nb::init<>())
			.def_rw("regions", &AnalyseRegionsResult::regions)
			.def_rw("z_roots", &AnalyseRegionsResult::zRoots)
			.def_rw("z_roots_multiplicity", &AnalyseRegionsResult::zRootsMultiplicity)
			.def_rw("z_poles", &AnalyseRegionsResult::zPoles)
			.def_rw("z_poles_multiplicity", &AnalyseRegionsResult::zPolesMultiplicity);

	// Binding GRPFAnalyse class
	nb::class_<GRPFAnalyse>(m, "GRPFAnalyse")
			.def(nb::init<const AnalyseParams &, std::string, bool>(),
			     "params"_a, "mesh_type"_a = "rect", "log"_a = false)
			.def(nb::init<std::function<std::complex<double>(std::complex<double>)>, const AnalyseParams &, std::string,
				     bool>(),
			     "func"_a, "params"_a, "mesh_type"_a = "rect", "log"_a = false)
			.def_prop_ro("num_nodes", &GRPFAnalyse::numNodes)
			.def("self_adaptive_run", &GRPFAnalyse::SelfAdaptiveRun)
			.def("triangulate", &GRPFAnalyse::Triangulate)
			.def("regular_grpf", &GRPFAnalyse::RegularGRPF)
			.def("adaptive_mesh_grpf", &GRPFAnalyse::AdaptiveMeshGRPF)
			.def("get_roots_and_poles", &GRPFAnalyse::GetRootsAndPoles)
			.def("generate_rectangle_mesh", &GRPFAnalyse::GenerateRectangleMesh)
			.def("generate_disk_mesh", &GRPFAnalyse::GenerateDiskMesh)
			.def("split_edge", &GRPFAnalyse::SplitEdge)
			.def("phase_analyse", &GRPFAnalyse::PhaseAnalyse)
			.def("evaluate_function", nb::overload_cast<const Eigen::ArrayXcd &>(&GRPFAnalyse::EvaluateFunction),
			     "new_function_values"_a)
			.def("evaluate_function", nb::overload_cast<>(&GRPFAnalyse::EvaluateFunction))
			.def("analyse_region", &GRPFAnalyse::AnalyseRegion)
			.def_rw("mode", &GRPFAnalyse::mode)
			.def_rw("function_values", &GRPFAnalyse::functionValues)
			.def_rw("params", &GRPFAnalyse::params)
			.def_rw("func", &GRPFAnalyse::func)
			.def_rw("it", &GRPFAnalyse::it)
			.def_rw("nodes_coord", &GRPFAnalyse::nodesCoord)
			.def_rw("new_nodes_coord", &GRPFAnalyse::newNodesCoord)
			// .def_rw("elements", &GRPFAnalyse::elements)
			// .def_rw("edges", &GRPFAnalyse::edges)
			.def_rw("phases_diff", &GRPFAnalyse::phasesDiff)
			.def_rw("quadrants", &GRPFAnalyse::quadrants)
			.def_rw("result", &GRPFAnalyse::result)
			.def_rw("regions", &GRPFAnalyse::regions);
}
