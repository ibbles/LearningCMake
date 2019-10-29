cmake_minimum_required(VERSION 3.15)
project(visMath VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=.;
    "-checks=*,-llvm-header-guard,-fuchsia-overloaded-operator,\
-google-readability-namespace-comments,-llvm-namespace-comment,\
-google-readability-namespace-comments,-google-build-using-namespace")


function(add_visMath_library NAME)
    add_library(${NAME} ${ARGN})
    target_include_directories(
        ${NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}include>
        $<INSTALL_INTERFACE:include>
    )
    target_compile_options(
        ${NAME} PRIVATE
        -Wall -Werror
    )
    target_compile_features(
        ${NAME} PRIVATE
        cxx_std_11  # Remove this PRIVATE if we ever include C++11 stuff in headers.
    )
endfunction()


add_visMath_library(visVector src/visVector.cpp)
add_visMath_library(visMatrix src/visMatrix.cpp)
add_visMath_library(visLinearAlgebra src/visLinearAlgebra.cpp)

target_link_libraries(visLinearAlgebra visVector visMatrix)

install(
    TARGETS visVector visMatrix
    EXPORT visMathTypesConfig
    DESTINATION lib
)

install(
    TARGETS visLinearAlgebra
    EXPORT visMathAlgorithmsConfig
    DESTINATION lib
)

install(
    EXPORT visMathTypesConfig
    NAMESPACE vis::
    DESTINATION lib/cmake
)

install(
    EXPORT visMathAlgorithmsConfig
    NAMESPACE vis::
    DESTINATION lib/cmake
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