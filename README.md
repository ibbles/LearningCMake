# Understanding CMake

CMake is a build system generator. That means that CMake is used in the build
process, but it doesn't actually run the compiler. Instead CMake generates
project files suitable for the current platform. It may be Makefile files, a
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

┌─────────┐
│ Target  │
├─────────┤
| Name    | To reference the target in CMake and filename of result. `-o` to GCC.
| Type    | Executable or library.
| Sources | List of .cpp files.
└─────────┘

The name will be used not only to name the compiled file on disk, but also to
refer to the target itself in CMake.

In addition to the source files, a compilation often require header files. The
source already specify which header file they need through `#include` directives
so we don't need to specify them again. However, the compiler needs to be told
where to look for the include files in other words it needs to know the include
directories. So we add this information to the build target.

┌───────────┐
│ Target    │
├───────────┤
| Name      |
| Type      |
| Sources   |
| Incl dirs | List of directories.
└───────────┘

There are several version of the C++ standard, such as C++ standard 11 and C++
standard 17, and the difference is sometimes important. We call the standard
that a compiler supports a feature of the compiler. There are other features as
well, but the language standard seems to cover most cases. But for correctness
let's call the target entry `cmplr ftrs`, short for `compiler features`.

┌───────────┐
│ Target    │
├───────────┤
| Name      |
| Type      |
| Sources   |
| Incl dirs |
| Cmpl ftrs | Feature requirements on the compiler, such as the C++ standard.
└───────────┘

Sometimes we need to pass additional compile options. These are
platform/compiler specific switches that CMake passes unchanged through the
build system to the compiler. Here we can include things such as warning level,
preprocessor defines, and architecture selectors such as `-mavx`.

┌───────────┐
│ Target    │
├───────────┤
| Name      |
| Type      |
| Sources   |
| Incl dirs |
| Cmpl ftrs |
| Cmpl opts | Compiler options passed unchanged to the compiler
└───────────┘

This is enough to build basic project that only consist of a single build
target, or rather projects that consist of independent build target. But in the
real world we often let one build target use functionality provided by another.
For example a library target and an executable target that links against the
library. This adds a bit of complexity since some, but not all, of the
properties we assign to the library should also apply to any target that link
with it. For example, a library may have public include directories that user
code must be able to include, but also have private include directories
containing internal headers that only the library source itself needs to see.
CMake models this separation by marking some properties as `INTERFACE`
properties, meaning that they should apply to users of the build target instead
of the target itself. The opposite is called `PRIVATE` properties. Of the
properties we've seen so far, this applies to the include directories, compile
features, and compile options. It is common that a property is included in both
the `INTERFACE` and `PRIVATE` sets.

┌───────────┐
│ Target    │
├───────────┤
| Name      |
| Type      |
| Sources   |
├───────────┤
| INTERFACE | Include directories, compile features, and compile options that
| Incl dirs | should be applied when building targets linking with this target.
| Cmpl ftrs |
| Cmpl opts |
├───────────┤
| PRIVATE   | Include directories, compile features, and compile options that
| Incl dirs | should be applied when building the target itself.
| Cmpl ftrs |
| Cmpl opts |
└───────────┘

When linking with a library the `INTERFACE` properties of that library become
`INTERFACE` properties of the linking target as well. Thus they will also be
included in a third target that link against the middle target in the chain. We
say that the `INTERFACE` properties are applied transitively.

The link target itself is also a property and is by default included in both the
`INTERFACE` and `PRIVATE` parts. This means that linking against a library that
in turn links against another library will cause the linker command for the
first target to also include the other library.

┌───────────┐
│ Target    │
├───────────┤
| Name      |
| Type      |
| Sources   |
├───────────┤
| INTERFACE |
| Incl dirs |
| Cmpl ftrs |
| Cmpl opts |
| Link libs | Extra libraries that users of this target link against.
├───────────┤
| PRIVATE   |
| Incl dirs |
| Cmpl ftrs |
| Cmpl opts |
| Link libs | Libraries that this target link against.
└───────────┘


## File locations

When using CMake we need to keep track of a number of different directories, or
directory trees, rather. Three important directory trees are the source three,
the binary tree, the install tree. The source tree is our code repository. This
is where we make our changes and it is this directory tree that is shared with
other people. CMake should only look at, but never touch, this directory. When
building a target the result of the build is stored in the binary tree. CMake
may store other files it needs here as well, such as generated project files and
intermediate build artifacts. When a build has been completed we may chose to
make an installation. An installation picks files from both the source tree and
the binary tree and copies them into the install tree. The install tree is a
self-contained version of the software we've built and is the directory that is
distributed to the end-user.

Header files are commonly included in both the source and the install directory
trees, but not always with the same layout. This means that the include
directories listed in the `INTERFACE` part of a target may need to be different
depending on if the target is being used from the source tree or the install
tree. We say that a particular include director is either part of the
`BUILD_INTERFACE` or the `INSTALL_INTERFACE`. I'm not sure if this applies to
`PRIVATE` include directories as well, but for now I'm only adding it to the
`INTERFACE` box below

┌───────────┐
│ Target    │
├───────────┤
| Name      |
| Type      |
| Sources   |
├───────────┤
| INTERFACE |
| Incl dirs |
|   build   | Include directories when building from the source tree.
|   install | Include directories when building from the install tree.
| Cmpl ftrs |
| Cmpl opts |
| Link libs |
├───────────┤
| PRIVATE   |
| Incl dirs |
| Cmpl ftrs |
| Cmpl opts |
| Link libs |
└───────────┘


## Installation

There are various things that can be installed. Two already mentioned are header
files and compiled binaries, such as executable programs and libraries. Where to
install the binaries is part of the target, but headers files aren't really part
of a target so they are handled separately. One way to do this is to request a
simple directory copy. We may, for example, say that the directory "include"
should be copied from the source tree to a directory with the same name in the
install tree.


┌───────────┐
│ Target    │
├───────────┤
| Name      | To reference the target in CMake and filename of result. `-o` to GCC.
| Type      | Executable or library.
| Sources   | List of .cpp files.
├───────────┤
| INTERFACE | Settings applied when building targets linking with this target.
| Incl dirs | List of include directories.
|   build   | Include directories when building from the source tree.
|   install | Include directories when building from the install tree.
| Cmpl ftrs | Feature requirements on the compiler, such as the C++ standard.
| Cmpl opts | Compiler options passed unchanged to the compiler
| Link libs | Extra libraries that users of this target link against.
├───────────┤
| PRIVATE   | Settings applied when building building the target itself.
| Incl dirs | List of include directories.
| Cmpl ftrs | Feature requirements on the compiler, such as the C++ standard.
| Cmpl opts |
| Link libs | Libraries that this target link against.
| Instl dir | Directory where the binaries this target generates will be installed.
└───────────┘

┌─────────────┐
│ Installs    │
├─────────────┤
| DIRECTORIES |
| include →   | Directory to copy from the source tree.
| include     | Directory that will be created in the install tree.
└─────────────┘


CMake generates the file`install_manifest.txt` which lists the files installed.

Binaries and headers are usually enough to use a library in another project, but
we would like our clients to be able to use all the meta data we packed into our
targets. The compiler flags and include paths etc. We even made the effort to
setup the `INSTALL_INTERFACE` for the include paths. To do this we need to
export this information so that it can be imported in the client project.
Whether or not a particular target should be exported is a property on that
target. The export property is not just a true/false flag, but a name of a group
of related targets that are installed and imported together. I'm not sure if
this property is in the `INTERFACE` or `PRIVATE` section of the target, but I
placed it in the `PRIVATE` part since users of this target doesn't automatically
become part of the same export group.

┌───────────┐
│ Target    │
├───────────┤
| Name      |
| Type      |
| Sources   |
├───────────┤
| INTERFACE |
| Incl dirs |
|   build   |
|   install |
| Cmpl ftrs |
| Cmpl opts |
| Link libs |
├───────────┤
| PRIVATE   |
| Incl dirs |
| Cmpl ftrs |
| Cmpl opts |
| Link libs |
| Instl dir |
| Export    | Export group that this target should be part of.
└───────────┘

When we have a collection of targets that should be exported together the next
step is to do the actual exporting. This is a mapping from an export group to a
file name and a directory in the install tree. This information goes along with
the rest of the non-target specific install information. CMake will allow you to
give any name to the export group, but it seems it must end with `Config`.
Otherwise the user project won't find it.

┌─────────────┐
│ Installs    │
├─────────────┤
| DIRECTORIES |
| include →   |
| include     |
| Export →    | Export all targets of this export group to
| Config file | this CMake configuration file in the install tree.
└─────────────┘



## Scripting

Before we get into writing all of this as a CMake project let's summarize the
information we need.


┌───────────┐
│ Target    │
├───────────┤
| Name      |
| Type      |
| Sources   |
├───────────┤
| INTERFACE |
| Incl dirs |
|   build   |
|   install |
| Cmpl ftrs |
| Cmpl opts |
| Link libs |
├───────────┤
| PRIVATE   |
| Incl dirs |
| Cmpl ftrs |
| Cmpl opts |
| Link libs |
| Instl dir |
| Export    | Export group that this target should be part of.
└───────────┘


┌─────────────┐
│ Installs    │
├─────────────┤
| DIRECTORIES |
| include →   |
| include     |
| Export →    | Export all targets of this export group to
| Config file | this CMake configuration file in the install tree.
└─────────────┘


We control CMake using one or more files named CMakeLists.txt which contains
CMake commands written in the CMake scripting language. We define one instance
in the chain above using, for example, the `add_executable` command:

```
add_executable(my_executable my_source_file.cpp)
```

