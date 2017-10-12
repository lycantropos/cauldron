import os
from conans import (ConanFile,
                    CMake)

__version__ = '0.0.0'
PROJECT_DIRECTORY = 'cauldron'


class CauldronConan(ConanFile):
    name = 'cauldron'
    version = __version__
    license = 'MIT'
    url = 'https://github.com/lycantropos/cauldron'
    description = 'Property-based testing inspired by "hypothesis".'
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {'shared': [True, False]}
    default_options = 'shared=False'
    generators = 'cmake'
    requires = 'catch/1.3.0@TyRoXx/stable'
    exports = '*.h', '*.cpp', 'CMakeLists.txt'

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy('*.h',
                  dst=os.path.join('include', PROJECT_DIRECTORY),
                  src=PROJECT_DIRECTORY)
        self.copy('*.cpp',
                  dst=os.path.join('include', PROJECT_DIRECTORY),
                  src=PROJECT_DIRECTORY)
        self.copy('*.dll', dst='bin', keep_path=False)
        self.copy('*.so', dst='lib', keep_path=False)
        self.copy('*.dylib', dst='lib', keep_path=False)
        self.copy('*.a', dst='lib', keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ['cauldron']
