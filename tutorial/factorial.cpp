#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <autocheck/sequence.hpp>
#include <autocheck/check.hpp>

std::size_t Factorial( const std::size_t number ) {
#define MAKE_IT_FAIL 1
#if MAKE_IT_FAIL
    return number <= 1 ? number : Factorial(number-1)*number;
#else
    return number > 1 ? Factorial(number-1)*number : 1;
#endif
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    SECTION( "Examples" ) {
        REQUIRE( Factorial(1) == 1 );
        REQUIRE( Factorial(2) == 2 );
        REQUIRE( Factorial(3) == 6 );
        REQUIRE( Factorial(10) == 3628800 );
    }
    SECTION( "Properties" ) {
        SECTION( "Factorial is always greater than 1" ) {
            autocheck::catch_reporter rep;
            autocheck::check<size_t>(
                [] (size_t x) { return Factorial(x) > 0; },
                100,
                autocheck::discard_if([](const size_t s){ return s > 30;},
                autocheck::make_arbitrary<size_t>()),
                rep);
        }
    }
}
