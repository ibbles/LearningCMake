cmake_minimum_required(VERSION 3.15)

project(visPhysics VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=.;
    "-checks=*,\
-fuchsia-overloaded-operator,\
-google-build-using-namespace,\
-google-readability-namespace-comments,\
-google-runtime-references,\
-llvm-header-guard,\
-llvm-namespace-comment,\
-misc-non-private-member-variables-in-classes,\
")

#list(APPEND CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake)

add_library(visPhysics src/visPhysics.cpp)

find_package(visMathTypes REQUIRED)
find_package(visMathAlgorithms REQUIRED)
target_link_libraries(visPhysics vis::visLinearAlgebra)

target_include_directories(visPhysics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE headers
)

target_compile_options(visPhysics
    PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:Clang>:-Wevereything -Wno-c++98-compat -Werror>
)

target_compile_features(visPhysics
    PRIVATE cxx_std_11  # Change to PUBLIC when C++11 stuff added to includes.
)

install(TARGETS visPhysics
    EXPORT visPhysicsTargets
    DESTINATION lib
)

install(EXPORT visPhysicsTargets
    FILE visPhysicsConfig.cmake
    NAMESPACE vis::
    DESTINATION cmake
)

install(
    DIRECTORY include/
    DESTINATION include
)

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_VERSION_MAJOR ${visPhysics_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${visPhysics_VERSION_MINOR})
include(CPack)