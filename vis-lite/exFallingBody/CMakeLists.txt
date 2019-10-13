cmake_minimum_required(VERSION 3.10)

project(exFallingBody VERSION 0.1)
option(MY_VERBOSE "Make version output extra verbose." ON)
if (MY_VERBOSE)
    set(MY_VERBOSE 1)
else()
    set(MY_VERBOSE 0)
endif()
configure_file(config.h.in include/config.h)

add_executable(exFallingBody exFallingBody.cpp)

target_include_directories(exFallingBody PUBLIC "${PROJECT_BINARY_DIR}/include")
# We don't expect our users to include config.h, but we still use PUBLIC because
# we expect to include "config.h" in header files that our users do include.
# Well, this is an executable so there aren't any "users" in that sense. I think
# we should have used PRIVATE.

set_target_properties(
    exFallingBody PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

install(TARGETS exFallingBody DESTINATION "bin")