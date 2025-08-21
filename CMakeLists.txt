cmake_minimum_required(VERSION 3.20)
project(grpfc)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

include_directories(eigen)

add_subdirectory(CDT/CDT CDT)
include_directories(CDT/CDT/include)

add_library(grpfc SHARED
    src/grpf.cpp
    src/meshing.cpp
    src/triangulation.cpp
    src/phase.cpp
    src/cauchy.cpp
    unittest/test_init_mesh.cpp
    src/analyse.cpp
    include/analyse.h
    include/grpf.h
    include/meshing.h
    include/triangulation.h
    include/phase.h
    include/cauchy.h
    include/utils.h
)

add_executable(main src/main.cpp)
target_link_libraries(main grpfc)

# Building examples
add_executable(example_rational_function
    examples/rational_function/analysis_setup.cpp
    include/gnuplot.h)
target_link_libraries(example_rational_function grpfc)

# Building unit tests
add_executable(test_init_mesh unittest/test_init_mesh.cpp)
target_link_libraries(test_init_mesh grpfc)