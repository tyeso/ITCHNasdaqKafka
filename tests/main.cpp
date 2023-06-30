#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only
                          // do this in one cpp file
#include <catch2/catch.hpp>
#include <misc.h>

TEST_CASE("Test printer function", "[print_hello_world]")
{
    REQUIRE(print_hello_world() == 1);
}
