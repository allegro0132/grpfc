cmake_minimum_required(VERSION 4.0)
project(grpfc)

set(CMAKE_CXX_STANDARD 20)

include_directories(eigen)

add_subdirectory(CDT/CDT CDT)
include_directories(CDT/CDT/include)

add_library(grpfc SHARED
    src/regular.cpp
    src/regular.h
    src/initial_mesh.cpp
    src/initial_mesh.h
    src/triangulation.cpp
    src/triangulation.h
    src/phase_grad.cpp
    src/phase_grad.h
    src/adaptive_mesh.cpp
    src/adaptive_mesh.h
    src/cauchy.cpp
    src/cauchy.h
    src/utils.h
    src/params.h
    unittest/test_init_mesh.cpp
    src/analyse.cpp
    src/analyse.h
)

add_executable(main src/main.cpp
    src/utils.h)
target_link_libraries(main grpfc)

# Building examples
add_executable(example_rational_function
    examples/rational_function/analysis_setup.cpp
    include/gnuplot.h)
target_link_libraries(example_rational_function grpfc)

# Building unit tests
add_executable(test_init_mesh unittest/test_init_mesh.cpp)
target_link_libraries(test_init_mesh grpfc)