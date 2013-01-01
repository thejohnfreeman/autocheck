Header-only C++11 library for QuickCheck (and later, SmallCheck) testing.

## Organization

- include\
  - autocheck\

    Main components:
    - **generator.hpp** : Generators produce an infinite sequence of sized values
      without fail. They can be mapped, filtered, or resized.
    - **arbitrary.hpp** : Generates a (possibly finite) sequence of tuples of
      function arguments (for the property to be tested) from a tuple of
      Generators.  Tuples can be filtered out and discarded. Generation fails
      after a maximum number of discards (default is no limit).
    - **reporter.hpp** : Reporters handle the response to test success and
      failure.
    - **check.hpp** : The test runner.

    Utility:
    - **value.hpp** : A generic value wrapper. The wrapped type does not need
      a default constructor, but `value` can hold one without allocating on
      the heap.

    Metaprogramming utilities:
    - **is_one_of.hpp** : Returns `std::true_type` if a type is found in a
      list of types. Used with `std::enable_if`.
    - **largest.hpp** : Returns the largest type (determined by `sizeof`) in
      a list of types. Used to implement `autocheck::value`.
    - **apply.hpp** : Calls a function with arguments pulled from a
      `std::tuple`.
    - **predicate.hpp** : Type function returning a `std::function` for a
      function taking some list of arguments and returning `bool`.

    Standard library extension:
    - **sequence.hpp** : Output stream operator for `std::vector`.
    - **tuple.hpp** : Output stream operator for `std::tuple`.

