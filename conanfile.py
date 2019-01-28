from conans import ConanFile

class AutocheckConan(ConanFile):
    name = 'autocheck'
    license = 'MIT'
    author = 'John Freeman <jfreeman08@gmail.com>'
    url = 'https://github.com/thejohnfreeman/autocheck'
    description = 'QuickCheck clone for C++'
    topics = ('testing')
    build_requires = (
        'Catch2/2.5.0@catchorg/stable',
        'gtest/1.8.1@bincrafters/stable',
    )
    generators = 'cmake_find_package'
    exports_sources = 'include/**/*.hpp'
    no_copy_source = True

    def package(self):
        self.copy('**/*.hpp')
