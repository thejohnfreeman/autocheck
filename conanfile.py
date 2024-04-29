from conan import ConanFile, conan_version
from conan.tools.cmake import CMake, cmake_layout

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

    requires = [
        # Available at https://conan.jfreeman.dev
        'cupcake.cmake/1.0.0@github/thejohnfreeman',
    ]
    generators = ['CMakeDeps', 'CMakeToolchain']

    exports_sources = [
        'CMakeLists.txt',
        'cupcake.json',
        'cmake/*',
        'external/*',
        'include/*',
        'src/*',
    ]

    # For out-of-source build.
    # https://docs.conan.io/en/latest/reference/build_helpers/cmake.html#configure
    no_copy_source = True

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        import json
        import pathlib
        path = pathlib.Path(self.recipe_folder) / 'cupcake.json'
        with path.open('r') as file:
            metadata = json.load(file)
        methods = {
            'tool': 'tool_requires',
            'test': 'test_requires',
        } if conan_version.major.value == 2 else {}
        for requirement in metadata.get('imports', []):
            groups = requirement.get('groups', [])
            group = groups[0] if len(groups) == 1 else 'main'
            method = methods.get(group, 'requires')
            getattr(self, method)(requirement['reference'])

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
