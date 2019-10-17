cmake_minimum_required(VERSION 3.10)

project(exFallingBody VERSION 0.1)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=.;
    "-checks=*,-llvm-header-guard,-fuchsia-overloaded-operator,\
-google-readability-namespace-comments,-llvm-namespace-comment,\
-google-readability-namespace-comments,-google-build-using-namespace")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/cmake)

add_executable(exFallingBody exFallingBody.cpp)

find_package(visMath REQUIRED)
find_package(visPhysics REQUIRED)
target_link_libraries(exFallingBody vis::visPhysics)

set_target_properties(
    exFallingBody PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

install(TARGETS exFallingBody DESTINATION "bin")