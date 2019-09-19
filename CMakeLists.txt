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