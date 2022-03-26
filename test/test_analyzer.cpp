#include "catch2/catch.hpp"

#include "analyzer.hpp"
#include <string>

using namespace std;

TEST_CASE("Test smileys", "[visitor]")
{
    string test_smiles = " :-] :{ :-[ :/ :) :-\\ :] :-{ :[ :-/ :-) :\\ ";
    REQUIRE(0 == 0);
}

TEST_CASE("Test top ten", "[visitor]")
{
    string test_text = ":\\hello:-] :{World:-[ :/ :) :-\\hello:] :-{World:[ :-/ :-) There are thousands of five-letter words in the English dictionary, but it only takes one to win Wordle. Whether it’s your first time playing, or you’re a seasoned Wordler who plays at midnight when a new word drops, these tips will help you build a strategy or improve upon one you’ve already created. Let’s get started.";
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
