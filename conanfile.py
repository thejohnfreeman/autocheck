from conans import python_requires

CMakeConanFile = python_requires('autorecipes/[*]@jfreeman/testing').cmake()

class Recipe(CMakeConanFile):
    name = CMakeConanFile.__dict__['name']
    version = CMakeConanFile.__dict__['version']
    topics = ('testing')
    settings = None
    build_requires = (
        'future/[*]@jfreeman/testing',
        'Catch2/2.5.0@catchorg/stable',
        'gtest/1.8.1@bincrafters/stable',
    )
    generators = 'cmake_find_package', 'cmake_paths'

    def package_id(self):
        return self.info.header_only()
