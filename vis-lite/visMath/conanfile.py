from conans import ConanFile, CMake, tools

class VisMathConan(ConanFile):
    name = "visMath"
    version = "0.2"
    url = ""
    license = ""
    description = "Basic math module for the VIS physics engine."
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"
    exports_sources = "*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build(target="install")

    def package_info(self):
        self.cpp_info.libs = ["visMath"]
