cmake_minimum_required(VERSION 3.10)

project(exFallingBody VERSION 0.1)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=.;
    "-checks=*,\
-cppcoreguidelines-avoid-magic-numbers,\
-fuchsia-overloaded-operator,\
-google-readability-namespace-comments,\
-google-build-using-namespace,\
-llvm-header-guard,\
-llvm-namespace-comment,\
-readability-magic-numbers,\
")

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake)

add_executable(exFallingBody exFallingBody.cpp)

find_package(visMathTypes REQUIRED)
find_package(visMathAlgorithms REQUIRED)
find_package(visPhysics REQUIRED)
target_link_libraries(exFallingBody vis::visPhysics)

set_target_properties(
    exFallingBody PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

install(TARGETS exFallingBody DESTINATION "bin")
