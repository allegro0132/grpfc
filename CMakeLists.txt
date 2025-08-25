cmake_minimum_required(VERSION 3.20...3.27)
project(grpfc)

if (CMAKE_VERSION VERSION_LESS 3.18)
  set(DEV_MODULE Development)
else()
  set(DEV_MODULE Development.Module)
endif()

find_package(Python 3.10 COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

# set up python bind
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
# Detect the installed nanobind package and import it into CMake
execute_process(
    COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)
find_package(nanobind CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

include_directories(eigen)

add_subdirectory(CDT/CDT CDT)
include_directories(CDT/CDT/include)

# Prepare library sources
set(src_grpfc
    src/grpf.cpp
    src/meshing.cpp
    src/triangulation.cpp
    src/phase.cpp
    src/cauchy.cpp
    unittest/test_init_mesh.cpp
    src/analyse.cpp
)

# Building Python module
nanobind_add_module(_grpfpy src/_grpfpy.cpp ${src_grpfc})

# Building shared library
add_library(grpfc SHARED ${src_grpfc})

# Building example
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