#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.h"
#include "testSupport.h"

ShellRunner r;

TEST_CASE( "Basic math tests", "[basicMath]" ) {
    SECTION( "Initialize") {
        REQUIRE( r.init() == true);
    }
    SECTION( "Integer Tests") {
        REQUIRE( r.runCommand("1 2 +", ".") == true);
        REQUIRE( r.getIntResult() == 3);
        REQUIRE( r.stacksEmpty() == true);
    }
    SECTION( "Float Tests") {
        REQUIRE( r.runCommand("1.0 2.0 f+", ".f") == true);
        REQUIRE( r.getFloatResult() == 3);
        REQUIRE( r.stacksEmpty() == true);
    }
    SECTION( "Eng Float Tests") {
        REQUIRE( r.runCommand("1.0 2.0 f+", ".e") == true);
        REQUIRE( r.getFloatResult() == 3);
        REQUIRE( r.stacksEmpty() == true);
    }
}
