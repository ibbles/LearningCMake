###
# Introduction to the basics.
###


# Set version of CMake to behave as.
cmake_minimum_required(
    VERSION 3.1..3.15)  # The minimum required version ... Highest version tested.
                        # Something about policies as well. Not sure on the details.

# Some information about the project.
project(
    MyProject  # Name of the project.
    VERSION 1.0 # Version of the project.
    DESCRIPTION "Very nice project"  # Description of the project.
    LANGUAGES CXX)  # List of languages used. Defaults to "C CXX".

# Create an executable from source.
add_executable(
    exe1  # Name of executable file generated and name of the CMake target.
    two.cpp three.h)  # List of files to include. Header files will not be compiled but included in IDEs.

# Create a library from source.
add_library(
    lib1  # Name of the library file generated and name of the CMake target.
    STATIC  # STATIC, SHARED or MODULE. Defaults to either STATIC or SHARED based on BUILD_SHARED_LIBS.
    two.cpp three.h)  # List of files to include.

# STATIC = Static library. .a on Linux, .lib without .dll on Windows.
# SHARED = Dynamicly linked library. .so on Linux, .lib/.dll on Windows.
# MODULE = Dynamicly linkede library that is not linked at compile time but with dlopen. Plugins.
# There is also INTERFACE which means that nothing needs to be compiled, for
# example a header-only library.
# There is also ALIAS which is a new name for an existing target.

# Tell the given target about a directory where header files are stored.
target_include_directories(
    exe1  # Target to modify.
    PUBLIC  # PUBLIC ⇒ Both this target and users need the directory.
            # PRIVATE ⇒ Only this target needs the directory.
            # INTERFACE ⇒ Only users needs the directory.
    include)  # The directory that should be passed to the compiler as a -I<directory>.

# Link exe1 to lib1.
target_link_libraries(
    exe1  # Target that should get the dependency.
    PUBLIC  # Or PRIVATE or INTERFACE. Same as for target_include_directories.
    lib1)  # Library to link to. Can be either a target, a library on disk, or a linker flag.

# Targets can have
# - include directories
# - linked libraries
# - compile options
# - compile features
# - and more.


# This project requires CMake version 3.8.
cmake_minimum_required(VERSION 3.8)

# This is a C++ project named Calculator.
project(Calculator LANGUAGES CXX)

# The project contains a static library called calclib built from calclib.cpp.
add_library(calclib STATIC src/calclib.cpp include/calc/lib.hpp)

# To build calclib and anything that uses it one need header files from the
# include directory.
target_include_directories(calclib PUBLIC include)

# To build calclib and anything that uses it one need a compiler that supports
# C++11.
target_compile_features(calclib PUBLIC cxx_std_11)

# The project contains an executable called calc built from calc.cpp.
add_executable(calc apps/calc.cpp)

# The executable need to use calclib, which means that it incorporates all the
# PUBLIC and INTERFACE requirements of calclib. In this case that means the
# include include directory and a C++11 compiler.
target_link_libraries(calc PUBLIC calclib)




###
# Variables and the cache
###


set(
    MY_VARIABLE  # Name of the variable. Usually all caps.
    "value")  # The value to give to the variable.

# Variables are accessable until the end of a function or file. The lifetime
# can be expanded one scope step by adding PARENT_SCOPE at the end.

set(
    MY_LIST  # Name of the variable holding the list.
    "one" "two")  # The elements of the list.

set(MY_LIST "one;two")  # This is an equivalent statement. That is, CMAKE doesn't
                        # really have lists but it can split strings at ';'.

# An unquoted value between whitespace is a string.
set(MY_LIST one two)  # Equivalent to set(MY_LIST "one" "two")
# The same is true for expanded variables.

set(MY_ELEMENTS "one two")  # I'm actually not sure what these produces.
set(MY_LIST ${MY_ELEMENTS}) # More testing is required.

# Printing a list prints the elements without separator.
# A string with spaces is converted to a list with separate_arguments.

set(MY_ELEMENTS "one two")
message(${MY_ELEMENTS})
# > one two
# Prints the string as it was passed to set. No surprices here.

set(MY_ELEMENTS one two)
message(${MY_ELEMENTS})
# > onetwo
# We gave two arguments to set, "one" and "two", so CMake interpreted this to
# mean a list of two elements. Elements are printed without separator, so the
# two words are concatenated together in the print.

set(MY_ELEMENTS "one;two")
message(${MY_ELEMENTS})
# > onetwo
# Here we only pass a single argument to set, but that parameter included the
# list separation token so CMake interpreted this to mean a list of two elements.

set(MY_ELEMENTS "one two")
set(MY_LIST ${MY_ELEMENTS})
message(${MY_LIST})
# > one two
# Creating a list from a string with spaces creates a list with one element
# containing the space. MY_LIST isn't really a list, it's just another STRING
# variables.

set(MY_ELEMENTS one two)
set(MY_LIST ${MY_ELEMENTS})
message(${MY_LIST})
# > onetwo
# Creating a list from another list. No surprices here.

set(MY_ELEMENTS "one;two")
set(MY_LIST ${MY_ELEMENTS})
message(${MY_LIST})
# > onetwo
# Basically the same. MY_ELEMENTS is a list and so is MY_LIST.

# The tutorial state that file system search path variables should always be
# quoted. The above testing indicate that this isn't necessary.
set(MY_PATH "/opt/my company/my application/my application")
message("Spaces are preserved here despite expanding without quotes:")
message(${MY_PATH})
# > Spaces are preserved here despite expanding without quotes:
# > /opt/my company/my application/my application

# Additionally, using quotes at the expand site doesn't help against missing
# quotes at the definition site:
set(MY_PATH /opt/my company/my application/my application)
message("Spaces are NOT preserved here despite the inclusion of quotes:")
message("${MY_PATH}")
# > Spaces are NOT preserved here despite the inclusing of quotes:
# > /opt/my;company/my;application/my;application
# This is because the ${MY_PATH} part of the code never sees the spaces. They
# have already been replaced with ';' when MY_PATH was first set.
# So when, exactly, are the quotes around ${MY_PATH} required?

set(
    MY_CACHE_VARIABLE  # The name of the variable. The same as regular set.
    "VALUE"  # The default value of the variable. Ignored if the variable already exists.
    CACHE  # Marker to indicate that the variable should be cached.
    STRING  # The type of the variable. Options are STRING, PATH, BOOL, ...
    "Description")  # Description to show in cmake-gui, ccmake, etc.

# One can add FORCE to the end to force the variable to take the new value even
# if it already exist in the cache. Not sure if the cache is updated with the
# new value, of it is set for this run only.

mark_as_advanced(MY_CACHE_VARIABLE)  # Hide the variable from cmake-gui, ccmake, etc.

# Passing INTERNAL as the type has has the same effect as if the type was STRING,
# FORCE was given, and mark_as_advanced called.
set(MY_CACHE_VARIABLE "VALUE" CACHE INTERNAL "Description")
# is the same as
set(MY_CACHE_VARIABLE "VALUE" CACHE STRING "Description" FORCE)
mark_as_advanced(MY_CACHE_VARIABLE)

# Booleans are often used so they have a short form.
option(MY_OPTION "Description" OFF)
# is the same as
set(MY_OPTION "OFF" CACHE BOOL "Description")

# ON and OFF are not the only valid boolean literals. Also TRUE and FALSE,

# System environment variables can be set with
set(ENV{variable_name} value)
# and read with
$ENV{variable_name}
# The recommendation is to avoid the usage of environment variables.

# The cache is stored in a text file named CMakeCache.txt in the build directory.

# Variables can be set not only in the current scope or in the cache, but also
# to items such as directories and targets. They are then call properties. Some
# are automatically created and initialized from a corrensponding global
# variable prefix with CMAKE_. Setting CMAKE_CXX_STANDARD causes new targets to
# be created with the CXX_STANDARD property set to that value.

set_property(
    TARGET target_name  # The target to set the property on.
    PROPERTY property_name  # The property that should be set.
    "value")  # The value the the property should be given.

set_target_properties(
    target_name  # The name to set the property on.
    PROPERTIES  # Keyworld to signal that the list of properties start.
    property_name  # The name of the property to set.
    "value")  # The value that the property should be give.
# The last two entries can be repeated with additional properties and values.

get_property(result_variable TARGET target_name PROPERTY property_name)
# After this ${result_variable} will be the value of the property.


###
# Programming in CMake
###

if (VARIABLE)
    # If VARIABLE is "ON", "YES", "TRUE", "Y", or non-zero number.
else()
    # If VARIABLES ins "OFF", "NO", "FALSE", "N", "0", "NOTFOUND", "",
    # or ends in "-NOTFOUND".
endif()
# If variables does not expand to one of the above, CMake will expand it then
# try again.

# I don't believe the last statment above, the one about expand and try again.
# In the below the if first sees the variable B and it is expanded to "A", which
# is neither in the TRUE list nor in the FALSE list. So the if should expand it
# and try again. By exanding "A" we get "OFF", which is in the FALSE list and
# the code should print "FALSE". It does not. Instead it prints TRUE. It seems
# the if interpreted the non-empty string "A" as TRUE and took the first block
# immediately.
set(A "OFF")
set(B A)
if (B)
  message("TRUE")
else ()
  message("FALSE")
endif()
# > TRUE

# Chained variable expansion is disabled when the variable is quoted, perhaps
# that has been expanded to unquoted variables as well since the tutorial was
# written.

# In addition to the list of TRUE/FALSE values there is a list of operator
# keywords: NOT, TARGET, EXISTS, DEFINED, STREQUAL, AND, OR, MATCHES,
# VERSION_LESS, VERSION_LESS_EQUAL, etc.
# Parenthesis can be used to group subexpressions.


# Generator-expressions

# Informational generator expressions are of the form `$<KEYWORD>`. They
# evaluate to a piece of information releveant for the current configuration.
# Another variant is `$<KEYWORD:value>`. The expression becomes `value` if
# KEYWORD is 1 and nothing if KEYWORD is 0.

# The inner generator-expression is $<CONFIG:Debug>
$<$<CONFIG:Debug>:--my-flag>

 # I dont understand this at all. The description makes no sense.


 # Macros and functions

# Macros set all it's variables in the parent scope while a function does not.

# Declare a new function named `my_function` that that two required arguments.
function(my_fuction arg1 arg2)
    message(${arg1})  # We expand parameters like any other variables.
    message(${arg2})

    # A common idiom is to let the caller pass in a variable name to which we
    # write the return value.
    set(${arg1} ${arg2} PARENT_SCOPE)
endfunction()

my_function(my_var "My string")
message("Variable set to ${my_var}.")

# Any arguments passed in addition to the once listed in the function signature
# are stored in the list ${ARGN}. All arguments, both listed and unlisted, are
# stored in ${ARGV}.


# Arguments


function(complex)
    # Generate a bunch of variables based on the arguments.
    cmake_parse_arguments(
        COMPLEX_PREFIX  # Prefix to give to all variable names.
        "SINGLE;ANOTHER"  # List of true/false flags.
        "ONE_VALUE;ALSO_ONE_VALUE" # List of single-word parameters.
        "MULTI_VALUES"  # List of list parameters.
        ${ARGN})

    message("${COMPLEX_PREFIX_SINGLE}")  # TRUE
    message("${COMPLEX_PREFIX_ANOTHER}")  # FALSE
    message("${COMPLEX_PREFIX_ONE_VALUE}")  # value
    message("${COMPLEX_PREFIX_ALSO_ONE_VALUE}")  #
    message("${COMPLEX_PREFIX_MULTI_VALUES}")  # some;other;values
endfunction()

complex(
    SINGLE  # By passing single we set it to TRUE.
            # By not passing ANOTHER we set it to FALSE
    ONE_VALUE value  # ONE_VALUE set to 'value'.
    MULTI_VALUES some other values)  # MULTI_VALUES set to the list "some;other;values".


###
# Communication with your code
###

# Configure files

# Used to write CMake variables into code by letting CMake generate the code.
# This is done by copying a .in file to non-.in file and doing variable
# substitution on its contents, where @ is used to mark variables.
# The copy-and-substitute is invoked usign the `configur_file` CMake command.

# Version.h.in:
#pragma once
#define MY_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define MY_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define MY_VERSION_PATCH @PROJECT_VERSION_PATCH@
#define MY_VERSION_TWEAK @PROJECT_VERSION_TWEAK@
#define MY_VERSION "@PROJECT_VERSION@"

# CMake:
configure_file (
    "${PROJECT_SOURCE_DIR}/include/MyLib/Version.h.in"
    "${PROJECT_BINARY_DIR}/include/MyLib/Version.h")


# Reading files

# If we wish to do this the other way around, having Version.h being the source
# of truth and CMake reading its variables from that, we would do the following:

set(VERSION_REGEX "#define MY_VERSION[ \t]+\"(.+)\"")
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/incldue/MyLib/Version.hpp"
    VERSION_STRING REGEX ${VERSION_REGEX})
string(REGEX REPLACE ${VERSION_REGEX} "\\1" VERSION_STRING "${VERSION_STRING}")
project(MyLib LANGUAGES CXX VERSION ${VERSION_STRING})


###
# How to structure you project
###

# Project/
#   .gitignore
#   README.md
#   LICENCE.md
#   CMakeLists.txt
#   cmake  # Add these with set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
#     FindSomeLib.cmake
#     something_else.cmake
#   include
#     project
#       lib.hpp
#   src
#     CMakeLists.txt
#     lib.cpp
#   apps
#     CMakeLists.txt
#     app.cpp
#   tests
#     CMakeLists.txt
#     testlib.cpp
#   docs
#     CMakeLists.txt
#   extern
#     googletest
#   scripts
#     helper.py

# We can make sure we are doing a out-of-source build using
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()


###
# Running other programs
###

# Running a command at configure time

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()


# Running a command at build time

find_package(PythonInterp REQUIRED)
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/include/Generated.hpp"
    COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/GenerateHeader.py" --argument
    DEPENDS some_target)

add_custom_target(generate_header ALL
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/include/Generated.hpp")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/Generated.hpp DESTINATION include)


###
# C++11 and beyond
###

# Two ways to select the C++ version to use.

# The first is
target_compile_features(myTarget PUBLIC cxx_std_11)
set_target_properties(myTarget PROPERTIES CXX_EXTENSIONS OFF)

# And the second is
set_target_properties(myTarget PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)



###
# Adding features
###

# Position independent code

# i.e., `-fPIC`. Usually automatically enabled for `SHARED` and `MODULE`
# libraries. Can be explictly enabled using
set_target_properties(lib1 PROPERTIES POSITION_INDEPENDENT_CODE ON)
# Not sure when one would need to do that though.


# Little libraries

# The `ld` library, which includes `ldopen` and `ldclose`, is included using
target_link_libraries(myTarget PUBLIC ${CMAKE_LD_LIBS})

# The `m` math library is included using
find_library(MATH_LIBRARY m)
if (MATH_LIBRARY)
    target_link_libraries(myTarget PUBLIC ${MATH_LIBRARY})
endif()



# Interprocedural optimization

# Also called link time optimization or `-flto`.
set_target_properties(myTarget PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)

###
# CCache and utilities
###

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()


# Utilities

# Set the following properties or CMAKE_* initializer variables to the command
# line for the tools. Most of them are limited to C or CXX with make or ninja
# generators.
#
#     <LANG>_CLANG_TIDY: CMake 3.6+
#     <LANG>_CPPCHECK
#     <LANG>_CPPLINT
#     <LANG>_INCLUDE_WHAT_YOU_USE

# Clang-tidy example:
if(CMAKE_VERSION VERSION_GREATER 3.6)
    # Add clang-tidy if available
    option(CLANG_TIDY_FIX "Perform fixes for Clang-Tidy" OFF)
    find_program(
        CLANG_TIDY_EXE
        NAMES "clang-tidy"
        DOC "Path to clang-tidy executable")

    if(CLANG_TIDY_EXE)
        if(CLANG_TIDY_FIX)
            set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}" "-fix")
        else()
            set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
        endif()
    endif()
endif()



###
# Exporting and installing
###


