# Understanding CMake

CMake is a build system generator. That means that CMake is used in the build
process, but it doesn't actually run the compiler. Instead CMake generates
project files suitable for the current platform. It may be Makefile files, a
Visual Studio project, or corresponding files for XCode. The flow is summarized
below:

Source files → CMake → Build system → Binary files.

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
| Cmpl opts |
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
of the target itself. The opposite is called `PRIVATE` properties. This applies
to the include directories, compile features, and compile options. It is common
that a property is included in both the `INTERFACE` and `PRIVATE` sets.

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

The link target itself is also a property, but it is always in the `INTERFACE`
part.

## Scripting

We control CMake using one or more files named CMakeLists.txt which contains
CMake commands written in the CMake scripting language. We define one instance
in the chain above using, for example, the `add_executable` command:

```
add_executable(my_executable my_source_file.cpp)
```

