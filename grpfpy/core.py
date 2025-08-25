import numpy as np
from grpfpy._grpfpy import AnalyseParams, AnalyseRegionsResult
from grpfpy._grpfpy import GRPFAnalyse as _GRPFAnalyse


class GRPFAnalyse:

    def __init__(self, func, params, mesh_type, log):
        self._self = _GRPFAnalyse(params, mesh_type, log)
        self.func = func
        self.log = log
        self._self.mode = 1
        self.params = params

    @property
    def result(self):
        return self._self.result

    @property
    def function_values(self):
        return self._self.function_values

    @property
    def it(self):
        return self._self.it

    @property
    def nodes_coord(self):
        return self._self.nodes_coord

    @property
    def new_nodes_coord(self):
        return self._self.new_nodes_coord

    @property
    def phases_diff(self):
        return self._self.phases_diff

    @property
    def quadrants(self):
        return self._self.quadrants

    @property
    def regions(self):
        return self._self.regions

    @property
    def num_nodes(self):
        return self._self.num_nodes

    def self_adaptive_run(self):
        while self._self.it < self._self.params.it_max and self._self.mode < 2:
            # Function evaluation
            new_z = (self._self.new_nodes_coord[:, 0] +
                     self._self.new_nodes_coord[:, 1] * 1j)
            new_function_values = self.func(new_z)
            self._self.evaluate_function(new_function_values)
            # Concat NodesCoord
            self._self.nodes_coord = np.vstack(
                [self._self.nodes_coord, self._self.new_nodes_coord])
            # Meshing operation
            self._self.triangulate()
            # Phase analysis
            self._self.phase_analyse()
            if self._self.mode == 0:
                self._self.adaptive_mesh_grpf()
            elif self._self.mode == 1:
                self._self.regular_grpf()
            # Split the edge in half
            self._self.split_edge()
            self._self.it += 1
            if self.log:
                print(f"Iteration: {self._self.it} done")
                print(
                    "----------------------------------------------------------"
                )

        # Final analysis
        if self.log:
            if self._self.mode == 2:
                print(f"Finish after: {self._self.it} iteration")
            elif self._self.mode == 3:
                print(
                    f"Assumed accuracy is achieved in iteration: {self._self.it}"
                )
        self._self.analyse_region()
        # Get Result
        self._self.get_roots_and_poles()
        return self._self.result


if __name__ == "__main__":

    def complex_fun(z):
        return (z - 1) * (z + 1) / (z + 1j)

    params = AnalyseParams(0.5, -2.0, 2.0, -2.0, 2.0, 1e-6, 0, float('inf'),
                           100)
    grpf = GRPFAnalyse(complex_fun, params, 'rect', False)
    grpf.self_adaptive_run()
    print(grpf.result.z_roots)
    print(grpf.result.z_poles)
