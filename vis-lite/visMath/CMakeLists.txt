cmake_minimum_required(VERSION 3.15)

project(visMath VERSION 0.2)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=.;
    "-checks=*,-llvm-header-guard,-fuchsia-overloaded-operator,\
-google-readability-namespace-comments,-llvm-namespace-comment,\
-google-readability-namespace-comments,-google-build-using-namespace")

add_library(visMath SHARED src/visMath.cpp)
target_include_directories(visMath PUBLIC "include")
install(TARGETS visMath DESTINATION lib)
install(DIRECTORY "include/" DESTINATION "include")

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/License.txt)
set(CPACK_PACKAGE_VERSION_MAJOR ${visMath_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${visMath_VERSION_MINOR})
set(CPACK_PACKAGE_CONTACT "ibbles@hotmail.com")  # Required by DEB packages.
include(CPack)