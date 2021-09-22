from conans import ConanFile, CMake
from os import environ

class DataConan(ConanFile):
    name = "data"
    version = "0.1"
    license = "MIT"
    author = "Daniel Krawisz"
    url = "https://github.com/DanielKrawisz/data"
    description = "Tools and high-level programming constructs in c++."
    topics = ("structures", "math", "encodings", "cryptography", "functional")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake_find_package"
    exports_sources = "*"
    requires = "boost/1.76.0", "openssl/1.1.1k", "cryptopp/8.5.0", "nlohmann_json/3.10.0", "gmp/6.2.1", "SECP256K1/0.1@proofofwork/stable"

    def set_version(self):
        if "CIRCLE_TAG" in environ:
            self.version = environ.get("CIRCLE_TAG")[1:]

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.hpp", dst="include", src="include")
        self.copy("*libdata.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["data"]
