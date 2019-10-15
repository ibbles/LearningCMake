cmake_minimum_required(VERSION 3.15)
project(visMath VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=.;
    "-checks=*,-llvm-header-guard,-fuchsia-overloaded-operator,\
-google-readability-namespace-comments,-llvm-namespace-comment,\
-google-readability-namespace-comments,-google-build-using-namespace")

add_library(
    visMath
    src/visMath.cpp
)

target_include_directories(
    visMath
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_options(
    visMath
    PRIVATE
    -Wall
    -Werror
)

target_compile_features(
    visMath
    PRIVATE  # Remove this PRIVATE if we ever include C++11 stuff in headers.
    cxx_std_11
)

install(
    TARGETS visMath
    EXPORT visMathTargets
    DESTINATION lib
)

install(
    EXPORT visMathTargets
    FILE visMathTargets.cmake
    NAMESPACE vis::
    DESTINATION cmake
)

install(
    DIRECTORY "include/"
    DESTINATION "include"
)

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_VERSION_MAJOR ${visMath_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${visMath_VERSION_MINOR})
set(CPACK_PACKAGE_CONTACT "ibbles@hotmail.com")  # Required by DEB packages.
include(CPack)