cmake_minimum_required(VERSION 3.15)

project(visPhysics VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=.;
    "-checks=*,-llvm-header-guard,-fuchsia-overloaded-operator,\
-google-readability-namespace-comments,-llvm-namespace-comment,\
-google-readability-namespace-comments,-google-build-using-namespace")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../installed/cmake)


add_library(visPhysics src/visPhysics.cpp)

find_package(visMath REQUIRED)
target_link_libraries(visPhysics vis::visMath)

target_include_directories(visPhysics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE headers
)

target_compile_options(visPhysics
    PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:Clang>:-Wevereything -Wno-c++98-compat>
)

target_compile_features(visPhysics
    PRIVATE cxx_std_11  # Change to PUBLIC when C++11 stuff added to includes.
)

install(TARGETS visPhysics
    EXPORT visPhysicsTargets
    DESTINATION lib
)

install(EXPORT visPhysicsTargets
    FILE visPhysicsExports.cmake
    NAMESPACE vis::
    DESTINATION cmake
)

install(DIRECTORY
    include
    DESTINATION include
)

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_VERSION_MAJOR ${visPhysics_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${visPhysics_VERSION_MINOR})
include(CPack)