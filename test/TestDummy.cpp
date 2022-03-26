#include "catch2/catch.hpp"

#include "analyzer.hpp"

SCENARIO("Dummy section")
{
    GIVEN("Nothing")
    {
        THEN("1 is always equal to 1")
        {
            REQUIRE(1 == 1);
        }
    }
}
