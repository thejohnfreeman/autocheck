from conan import ConanFile
from conan.tools.cmake import CMake

class Autocheck(ConanFile):
    name = 'autocheck'
    version = '1.0.0'
    user = 'github'
    channel = 'thejohnfreeman'

    license = 'ISC'
    author = 'John Freeman <jfreeman08@gmail.com>'

    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {'shared': [True, False], 'fPIC': [True, False]}
    default_options = {'shared': False, 'fPIC': True}

    requires = ['cupcake/0.4.1@github/thejohnfreeman']
    test_requires = ['catch2/3.3.2', 'gtest/1.13.0']
    generators = ['CMakeDeps', 'CMakeToolchain']

    exports_sources = [
        'CMakeLists.txt',
        'cmake/*',
        'external/*',
        'include/*',
        'src/*',
    ]

    # For out-of-source build.
    # https://docs.conan.io/en/latest/reference/build_helpers/cmake.html#configure
    no_copy_source = True

    def requirements(self):
        for requirement in self.test_requires:
            self.requires(requirement)

    def config_options(self):
        if self.settings.os == 'Windows':
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={'BUILD_TESTING': 'NO'})
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        path = f'{self.package_folder}/share/{self.name}/cpp_info.py'
        with open(path, 'r') as file:
            exec(file.read(), {}, {'self': self.cpp_info})
