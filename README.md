# Understanding CMake

CMake is a build system generator. That means that CMake is used in the build
process, but it doesn't actually run the compiler. Instead CMake generates
project files suitable for the current platform. It may be a set of Makefiles, a
Visual Studio project, or corresponding files for XCode. The flow is summarized
below:

Source files → CMake → Build system → Binary files.


## Targets and its properties

The main building block in CMake is a build target. A build target represents
one strand through the flow above. It (often) starts with a set of source files
and (often) ends with a compiled binary. The binary can be an executable or a
library. A single CMake project typically contains several build targets, often
with dependencies between them. To create a build target we need to supply at
least three pieces of information: the source files, if the target is an
executable or a library, and the name of the binary. So we have this:

```
┌─────────┐
│ Target  │
├─────────┤
| Name    | To reference the target in CMake and filename of the final file.
| Type    | Executable or library.
| Sources | List of .cpp files.
└─────────┘
```

The name will be used not only to name the compiled file on disk, but also to
refer to the target itself in CMake.

In addition to the source files, a compilation often require header files. The
source files already specify which header file they need through `#include`
directives so we don't need to specify them again. However, the compiler needs
to be told where to look for the include files. In other words it needs to know
the include directories. So we add this information to the build target.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                |
| Type                |
| Sources             |
| Include directories | List of directories.
└─────────────────────┘
```

There are several version of the C++ standard, such as C++ standard 11 and C++
standard 17, and the difference is sometimes important. We call the standard
that a compiler supports a feature of the compiler. There are other features as
well, but the language standard seems to cover most cases. Regardless, for
correctness let's call the target entry `Compiler features`.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                |
| Type                |
| Sources             |
| Include directories |
| Compiler features   | Feature requirements on the compiler, such as the C++ standard.
└─────────────────────┘
```

Sometimes we need to pass additional compile options. These are
platform/compiler specific switches that CMake passes unchanged through the
build system to the compiler. Here we can include things such as warning level,
preprocessor defines, and architecture selectors such as `-mavx`.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                |
| Type                |
| Sources             |
| Include directories |
| Compiler features   |
| Compiler options    | Compiler options passed unchanged to the compiler
└─────────────────────┘
```

This is enough to build basic project that only consist of a single build
target, or rather projects that consist of independent build targets. But in the
real world we often let one build target use functionality provided by another.
For example a library target and an executable target that links against the
library. This adds a bit of complexity since some, but not all, of the
properties we assign to the library should also apply to any target that link
with it. For example, a library may have include directories that are part of
the interface and that user code must be able to include, but also have private
include directories containing internal headers that only the library source
itself needs to see. CMake models this separation by marking some properties as
`INTERFACE` properties, meaning that they should apply to users of the build
target instead of the target itself, while the opposite is called `PRIVATE`
properties. Of the properties we've seen so far, this applies to the include
directories, compile features, and compile options. It is common that a property
is included in both the `INTERFACE` and `PRIVATE` sets. CMake calls such
properties `PUBLIC` properties.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                |
| Type                |
| Sources             |
├─────────────────────┤
| INTERFACE           | Include directories, compile features, and compile
| Include directories | options that should be applied when building targets
| Compiler features   | linking with this target.
| Compiler options    |
├─────────────────────┤
| PRIVATE             | Include directories, compile features, and compile
| Include directories | options that should be applied when building the target
| Compiler features   | itself.
| Compiler options    |
└─────────────────────┘
```

When linking with a library the `INTERFACE` properties of that library become
`INTERFACE` properties of the linking target as well. Thus they will also be
included in a third target that link against the middle target in the chain. We
say that the `INTERFACE` properties are applied transitively.

The link target itself is also a property and is by default included in both the
`INTERFACE` and `PRIVATE` parts. This means that linking against a library that
in turn links against another library will cause the linker command for the
first target to also include the other library.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                |
| Type                |
| Sources             |
├─────────────────────┤
| INTERFACE           |
| Include directories |
| Compiler features   |
| Compiler options    |
| Link libraries      | Extra libraries that users of this target link against.
├─────────────────────┤
| PRIVATE             |
| Include directories |
| Compiler features   |
| Compiler options    |
| Link libraries      | Libraries that this target link against.
└─────────────────────┘
```

The above is all the information we need to supply in order to build most
projects.


## File locations

When using CMake we need to keep track of a number of different directories, or
directory trees rather. Three important directory trees are the source three,
the binary tree, and the install tree. The source tree is our code repository.
This is where we make our changes and it is this directory tree that is shared
with other people. CMake should only look at, but never touch, this directory.
When building a target the result of the build is stored in the binary tree.
CMake may store other files it needs here as well, such as generated project
files and intermediate build artifacts. When a build has been completed we may
chose to make an installation. An installation picks files from both the source
tree and the binary tree and copies them into the install tree. The install tree
is a self-contained version of the software we've built and is the directory
that is distributed to the end-user.

Header files are commonly included in both the source and the install directory
trees, but not always with the same layout. This means that the include
directories listed in the `INTERFACE` part of a target may need to be different
depending on if the target is being used from the source tree or the install
tree. We say that a particular include director is either part of the
`BUILD_INTERFACE` or the `INSTALL_INTERFACE`. I'm not sure if this applies to
`PRIVATE` include directories as well, but for now I'm only adding it to the
`INTERFACE` box below

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                |
| Type                |
| Sources             |
├─────────────────────┤
| INTERFACE           |
| Include directories |
|   build             | Include directories when building from the source tree.
|   install           | Include directories when building from the install tree.
| Compiler features   |
| Compiler options    |
| Link libraries      |
├─────────────────────┤
| PRIVATE             |
| Include directories |
| Compiler features   |
| Compiler options    |
| Link libraries      |
└─────────────────────┘
```


## Installation

There are various things that can be installed. Two already mentioned are header
files and compiled binaries, such as executable programs and libraries. Where to
install the binaries is part of the target, but headers files aren't really part
of a target so they are handled separately. One way to do this is to request a
simple directory copy. We may, for example, say that the directory "include"
should be copied from the source tree to a directory with the same name in the
install tree.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                |
| Type                |
| Sources             |
├─────────────────────┤
| INTERFACE           |
| Include directories |
|   build             |
|   install           |
| Compiler features   |
| Compiler options    |
| Link libraries      |
├─────────────────────┤
| PRIVATE             |
| Include directories |
| Compiler features   |
| Compiler options    |
| Link libraries      |
| Install directory   | Directory where the binaries this target generates will be installed.
└─────────────────────┘

┌────────────────────┐
│ Installs           │
├────────────────────┤
| DIRECTORIES        |
| include → include  | Directory to copy from the source tree to the install tree.
└────────────────────┘
```

CMake generates the file`install_manifest.txt` which lists the files installed.

Binaries and headers are usually enough to use a library in another project, but
we would like our clients to be able to use all the meta data we packed into our
targets. The compiler flags, include paths, compile options, etc. We even made
the effort to setup the `INSTALL_INTERFACE` for the include paths for this exact
purpose. To do this we need to export this information so that it can be
imported in the client project. Whether or not a particular target should be
exported is a property on that target. The export property is not just a
true/false flag, but a name of a group of related targets that are installed and
imported together. I'm not sure if this property is in the `INTERFACE` or
`PRIVATE` section of the target, but I placed it in the `PRIVATE` part since
users of this target doesn't automatically become part of the same export group.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                |
| Type                |
| Sources             |
├─────────────────────┤
| INTERFACE           |
| Include directories |
|   build             |
|   install           |
| Compiler features   |
| Compiler options    |
| Link libraries      |
├─────────────────────┤
| PRIVATE             |
| Include directories |
| Compiler features   |
| Compiler options    |
| Link libraries      |
| Install directory   |
| Export group        | Export group that this target should be part of.
└─────────────────────┘
```

When we have a collection of targets that should be exported together the next
step is to do the actual exporting. This is a mapping from an export group to a
file name and a directory in the install tree. This information goes along with
the rest of the non-target specific install information. CMake will allow you to
give any name to the export group, but it seems it must end with `Config`.
Otherwise the user project won't find it.

```
┌──────────────────────────────┐
│ Installs                     │
├──────────────────────────────┤
| DIRECTORIES                  |
| include → include            |
| Export group → Config file   | Export all targets of this export group to this CMake configuration file in the install tree.
└──────────────────────────────┘
```


## Scripting

Before we get into writing all of this as a CMake project let's summarize the
information we need.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                |
| Type                |
| Sources             |
├─────────────────────┤
| INTERFACE           |
| Include directories |
|   build             |
|   install           |
| Compiler features   |
| Compiler options    |
| Link libraries      |
├─────────────────────┤
| PRIVATE             |
| Include directories |
| Compiler features   |
| Compiler options    |
| Link libraries      |
| Install directory   |
| Export group        |
└─────────────────────┘


┌──────────────────────────────┐
│ Installs                     │
├──────────────────────────────┤
| DIRECTORIES                  |
| include → include            |
| Export group → Config file   |
└──────────────────────────────┘
```

We control CMake using one or more files named CMakeLists.txt which contains
CMake commands written in the CMake scripting language. We define a target
using either the `add_executable` command or the `add_library` command:

```
add_executable(<target_name> <source_file> [<source_file>]...)
add_library(<target_name> [<link_type>] <source_file> [<source_file>]...)
```

The link type, either `STATIC`, `SHARED`, or `MODULE`, is optional and not
specifying it allows the builder, i.e., the developer running `CMake`, to chose.

Lets make an example shared library. The `my_` prefix will be used for all
user-supplied names.

```
add_library(my_math my_add.cpp my_sub.cpp)
```

After this call we have the following information in our target, given along
with a note on which CMake command was used to provide the information.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                | add_library
|  my_math            |
| Type                | add_library
|  Shared library     |
| Sources             | add_library
|  my_add.cpp         |
|  my_sub.cpp         |
└─────────────────────┘
```

The next piece of information on the list is the include directories. Naturally,
we add include directories to a target using the `target_include_directories`
command.

```
target_include_directories(<target> INTERFACE|PRIVATE|PUBLIC <directory> [<directory>]...)
```

For example:

```
target_include_directories(my_math PRIVATE my_source)

target_include_directories(
    my_math
    PUBLIC
    $<BUILD_INTERFACE:my_include>
    $<INSTALL_INTERFACE:my_math/include>)
```

Here we say that when building `my_math` itself we should look for headers in
both `my_source` and `my_include`. When building other targets that are part of
the same project, i.e., when using the `BUILD_INTERFACE`, we should look for
headers in `my_include`, and lastly, when building other targets using an
installed version of `my_math` we should look for headers in `my_math/include`.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                | add_library
|   my_math           |
| Type                | add_library
|   Shared library    |
| Sources             | add_library
|   my_add.cpp        |
|   my_sub.cpp        |
├─────────────────────┤
| INTERFACE           |
| Include directories | target_include_directories
|   build             |
|     my_include      |
|   install           |
|  my_math/my_include |
├─────────────────────┤
| PRIVATE             |
| Include directories | target_include_directories
|   my_include        |
|   my_source         |
└─────────────────────┘
```

Next up is compiler features, which we add to a target using
`target_compile_features`.


```
target_compile_feature(<target> INTERFACE|PUBLIC|PRIVATE <feature> [<feature> ...])
```

```
target_compile_feature(my_math PRIVATE cxx_std_11)
```

The example above state that to build the `my_math` target the compiler must
support the C++ 11 standard. There is no such requirement on users of the
`my_math` library since the `PRIVATE` scope keyword was used.

The compiler options are handle in a very similar way.

```
target_compile_options(<target> INTERFACE|PUBLIC|PRIVATE <option> [<option> ...])
```

```
target_compile_options(my_math PRIVATE -Wall -Werror)
```

```
Here we add some warning related compiler options.

┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                | add_library
|   my_math           |
| Type                | add_library
|   Shared library    |
| Sources             | add_library
|   my_add.cpp        |
|   my_sub.cpp        |
├─────────────────────┤
| INTERFACE           |
| Include directories | target_include_directories
|   build             |
|     my_include      |
|   install           |
|  my_math/my_include |
| Compiler features   | target_compile_features
| Compiler options    | target_compile_options
├─────────────────────┤
| PRIVATE             |
| Include directories | target_include_directories
|   my_include        |
|   my_source         |
| Compiler features   | target_compile_features
| Compiler options    | target_compile_options
└─────────────────────┘
```


To link with a library we use `target_link_libraries`.

```
target_link_libraries(<target> PRIVATE|PUBLIC|INTERFACE <library> [<library> ...])
```

```
target_link_libraries(my_calculator PUBLIC my_math)
```

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                | add_library
|   my_math           |
| Type                | add_library
|   Shared library    |
| Sources             | add_library
|   my_add.cpp        |
|   my_sub.cpp        |
├─────────────────────┤
| INTERFACE           |
| Include directories | target_include_directories
|   build             |
|     my_include      |
|   install           |
|  my_math/my_include |
| Compiler features   | target_compile_features
| Compiler options    | target_compile_options
| Link libraries      | target_link_libraries
├─────────────────────┤
| PRIVATE             |
| Include directories | target_include_directories
|   my_include        |
|   my_source         |
| Compiler features   | target_compile_features
| Compiler options    | target_compile_options
| Link libraries      | target_link_libraries
└─────────────────────┘
```

A pattern is emerging. After having created a target, adding properties to this
target often follows the the following template:

```
target_<property>(<target> PRIVATE|PUBLIC|INTERFACE <item> [<item> ...])
```

where `<property>` is one of `include_directories`, `compile_features`,
`compile_options`, or `link_libraries`, and `<item>` is the include director,
compiler feature, compiler option, or link library we wish to add to the target.

Unfortunately, the install configuration doesn't follow this template and is a
bit more complicated. There are four pieces of information we must provide. The
first is the directory that the binaries that the target produces should be
installed. The second is the export group that the target should be part of. The
third is the `my_Config.cmake` file that the export group should be written to,
and lastly is any other files or directories that should be installed.

The first two, binaries directory and export group, are specified using the same
CMake command.

```
install(TARGETS <target>
    EXPORT <export group>
    RUNTIME DESTINATION <path>
    LIBRARY DESTINATION <path>
    ARCHIVE DESTINATION <path>)
```

```
install(
    TARGETS my_math
    EXPORT my_mathConfig
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
```

Here we say that the target `my_math` is part of the `my_mathConfig` export
group and we list a bunch of install destinations. There is a "correct" way to
set these and the defaults for the current platform is used when `install` is
given as follows.

```
include(GNUInstallDirs)
install(TARGETS <target>
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
```

Don't read too much into the `GNU` part of `GNUInstallDirs`. It says more about
the history of the file that what it actually does.

This has become so common that starting with CMake 3.14 the above has become the
default when simply giving `install(TARGETS <target>)`. Just don't forget to
also include the `EXPORT` part.

The last two, the `my_Config.cmake` file and other files and directories, are
not related to the target. We specify that a `my_Config.cmake` file should be
generated with the following.

```
install(
    EXPORT <export group>
    NAMESPACE <name>::
    DESTINATION <path>
)
```

```
install(
    EXPORT my_mathConfig
    NAMESPACE my_::
    DESTINATION cmake
)
```

The `NAMESPACE` part is a prefix that is added to all targets in the export
group when they are imported into another project. The purpose is to avoid name
conflicts. The `DESTINATION` is the directory that the `my_Config.cmake` files
should be installed to. We can use `GNUInstallDirs` here as well by setting
`DESTINATION` to `${CMAKE_INSTALL_LIBDIR}/cmake/my_math`.

Files and directories are installed with

```
install(DIRECTORY <source directory> DESTINATION <destination directory>)
```

```
install(DIRECTORY "include/" DESTINATION "include")
```

There is some trickery going on with the ending `/` in the source directory. If
we include the `/`, then contents of the directory is copied to the destination,
while if we don't include it then the directory itself is copied.

The install is now complete and we have the following.

```
┌─────────────────────┐
│ Target              │
├─────────────────────┤
| Name                | add_library
|   my_math           |
| Type                | add_library
|   Shared library    |
| Sources             | add_library
|   my_add.cpp        |
|   my_sub.cpp        |
├─────────────────────┤
| INTERFACE           |
| Include directories | target_include_directories
|   build             |
|     my_include      |
|   install           |
|     my_math/my_include |
| Compiler features   | target_compile_features
| Compiler options    | target_compile_options
| Link libraries      | target_link_libraries
├─────────────────────┤
| PRIVATE             |
| Include directories | target_include_directories
|   my_include        |
|   my_source         |
| Compiler features   | target_compile_features
| Compiler options    | target_compile_options
| Link libraries      | target_link_libraries
| Install directory   | install(TARGETS <target> DESTINATION)
| Export group        | install(TARGETS <target> EXPORT)
└─────────────────────┘


┌──────────────────────────────┐
│ Installs                     │
├──────────────────────────────┤
| DIRECTORIES                  |
| include → include            | install(DIRECTORY)
| Export group → Config file   | install(EXPORT)
└──────────────────────────────┘
```


## Import

This section describe how to use installed and exported targets and files in
another project. Since we aren't building the the target in question we're only
interested in the `INTERFACE` part of the properties. This is exactly the
information that is made available in the `my_Config.cmake` file. We just need a
way to make the new CMake context aware of it. I have yet to find a way to do
this in a way that doesn't require hard coding search paths. The
`my_Config.cmake` file is a CMake module and we use the `CMAKE_MODULE_PATH`
variable tell CMake where to look for modules. We simply append the directory
containing `my_Config.cmake` to it.


_Note to self. I'm not sure if it should be `CMAKE_MODULE_PATH` or
`CMAKE_PREFIX_PATH`. Yesterday it worked with `CMAKE_MODULE_PATH`, but today I
must use `CMAKE_PREFIX_PATH`. I wonder what it will be tomorrow._
```
list(APPEND CMAKE_MODULE_PATH <path>)
list(APPEND CMAKE_PREFIX_PATH <path>)
```

The difficult part is to know what to put in place of `<path>`. It seems we need
to know this ourselves somehow and either hard-coding it into the client
`CMakeLists.txt` or by letting the user specify in on the command line when
invoking CMake. I typically follow the convention that all projects have the
same `${CMAKE_INSTALL_PREFIX}`, and CMake modules, such as the `my_Config.cmake`
files, are placed in the `lib/cmake` subdirectory. Then we can use the following
update to `CMAKE_MODULE_PATH` in any projects.

```
list(APPEND CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake)
```

To use the installed library the only thing we need to do is

```
find_package(<package> REQUIRED)
target_link_libraries(<target> <namespace>::<target>)
```

This will handle everything. Include paths will be added, compiler features and
options will be enforced, and the linker command line will be updated to include
the new library and all its dependencies. Well, that's the intention at least.
Doesn't seem to work though.

