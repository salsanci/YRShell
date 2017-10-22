#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.h"
#include "testSupport.h"

ShellRunner r;

TEST_CASE( "Basic math tests", "[basicMath]" ) {
    SECTION( "Integer Tests") {
        REQUIRE( r.runCommand("5 -5 8 1 2 + swap - swap * swap /", ".") == true);
        REQUIRE( r.getIntResult() == 5);
        REQUIRE( r.stacksEmpty() == true);
    }
    SECTION( "Float Tests") {
        REQUIRE( r.runCommand("5.0 -5.0 8.0 1.0 2.0 f+ swap f- swap f* swap f/", ".f") == true);
        REQUIRE( r.getFloatResult() == 5);
        REQUIRE( r.stacksEmpty() == true);
    }
    SECTION( "Eng Float Tests") {
        REQUIRE( r.runCommand("5.0 -5.0 8.0 1.0 2.0 f+ swap f- swap f* swap f/", ".e") == true);
        REQUIRE( r.getFloatResult() == 5);
        REQUIRE( r.stacksEmpty() == true);
    }
}
TEST_CASE( "Basic compile tests", "[basicCompile]" ) {
    SECTION( "Compile Tests") {
        REQUIRE( r.runCommand(": f1 + swap - swap * swap /", "") == true);
        REQUIRE( r.runCommand(": f2 f+ swap f- swap f* swap f/", "") == true);
    }
    SECTION( "Use f1 Integer Test") {
        REQUIRE( r.runCommand("5 -5 8 1 2 f1", ".") == true);
        REQUIRE( r.getIntResult() == 5);
        REQUIRE( r.stacksEmpty() == true);
    }
    SECTION( "Use f2 Float Tests") {
        REQUIRE( r.runCommand("5.0 -5.0 8.0 1.0 2.0 f2", ".f") == true);
        REQUIRE( r.getFloatResult() == 5);
        REQUIRE( r.stacksEmpty() == true);
    }
    SECTION( "Use f2 Eng Float Tests") {
        REQUIRE( r.runCommand("5.0 -5.0 8.0 1.0 2.0 f2", ".e") == true);
        REQUIRE( r.getFloatResult() == 5);
        REQUIRE( r.stacksEmpty() == true);
    }
}
