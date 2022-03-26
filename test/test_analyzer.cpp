#include "catch2/catch.hpp"

#include "analyzer.hpp"

TEST_CASE("Test with zero", "[classic]")
{
  
   REQUIRE(0 == 0);
}

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
