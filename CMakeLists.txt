# Set version of CMake to behave as.
cmake_minimum_required(
    VERSION 3.1..3.15)  # The minimum required version ... Highest version tested.

# Some information about the project.
project(
    MyProject  # Name of the project.
    VERSION 1.0 # Version of the project.
    DESCRIPTION "Very nice project"  # Description of the project.
    LANGUAGES CXX)  # List of languages used.

# Create an executable from source.
add_executable(
    exe1  # Name of executable file generated and name of the CMAke target.
    two.cpp three.h)  # List of files to include. Header files will not be compiled but included in IDEs.

# Create a library from source.
add_library(
    lib1  #
    STATIC  # STATIC, SHARED or MODULE. Defaults to either STATIC or SHARED based on BUILD_SHARED_LIBS.
    two.cpp three.h)  # List of files to include.

# There is also INTERFACE which means that nothing needs to be compiled, for
# example a header-only library.
# There is also ALIAS which is a new name for an existing target.

# Make
target_include_directories(
    exe1  # Target to modify.
    PUBLIC  # PUBLIC ⇒ Both this target and users need directory.
            # PRIVATE ⇒ Only this target needs directory.
            # INTERFACE ⇒ Only users needs directory.
    include)  # The directory that should be passed to the compiler as a -I<directory>.

# Link exe1 to lib1.
target_link_libraries(
    exe1  # Target that should get the dependency.
    PUBLIC  # Or PRIVATE or INTERFACE.
    lib1)  # Library to link to. Can be either a target, a library on disk, or a linker flag.

# Targets can have
# - include directories
# - linked libraries
# - compile options
# - compile features
# -  and more.


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

set(MY_ELEMENTS "one two")
set(MY_LIST ${MY_ELEMENTS})

# Printing a list prints the elements without separator.
# A string with spaces is converted to a list with separate_arguments.

set(
    MY_CACHE_VARIABLE  # The name of the variable. The the same as regular set.
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
set(MY_CACHE_VARIABLE "VALUE" CACHE STRING "Description")
mark_as_advanced(MY_CACHE_VARIABLE)

# Booleans are often used so they have a short form.
option(MY_OPTION "Description" OFF)
# is the same as
set(MY_OPTION "OFF" CACHE BOOL "Description")

# ON and OFF are not the only valid boolean literals. Aldo TRUE and FALSE,

# System environment variables can be set with
set(ENV{variable_name} value)
# and read with
$ENV{variable_name}
# The recommendation is to avoid the usage of environment variables.

# The cache is stored in a text file name CMAkeCache.txt in the build directory.

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