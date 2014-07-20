#define CATCH_CONFIG_MAIN
#include <CATCH/catch.hpp>

#include "TTL/TTL.hpp"


TEST_CASE ( "Argument Parser is Tested", "[Argument]" )
{
    ttl::Argument arg;

    arg.setInert('f');
    arg.setInert('d');
    arg.setInert("verbose");

    arg.pass("/home/user/development/program --geometry=800x600 -fvd 5 folder/filename system -q --verbose atom -- -p control data");

    REQUIRE ( arg.isInert('f') );
    REQUIRE ( arg.isInert('d') );
    REQUIRE ( arg.isInert("verbose") );

    REQUIRE ( arg.getOperandCount() == 5 );

    REQUIRE ( arg.getPath() == "/home/user/development/program" );

    REQUIRE ( arg.isPassed("geometry") );
    REQUIRE ( arg.getArgument("geometry") == "800x600" );

    REQUIRE ( arg.isPassed('f') );
    REQUIRE ( arg.isPassed('d') );

    REQUIRE ( arg.isPassed('v') );
    REQUIRE ( arg.getArgument('v') == "5" );

    REQUIRE ( arg.getOperand(0) == "folder/filename" );
    REQUIRE ( arg.getOperand(1) == "system" );

    REQUIRE ( arg.isPassed('q') );
    REQUIRE ( arg.isPassed("verbose") );

    REQUIRE ( arg.getOperand(2) == "-p" );
    REQUIRE ( arg.getOperand(3) == "control" );
    REQUIRE ( arg.getOperand(4) == "data" );

    REQUIRE ( arg.isInert('q') == false );
    REQUIRE ( arg.getArgument('q') == "atom" );


//    SECTION ( "Parsing arguments directly from the command line" )
//    {
//        ttl::Argument arguments();
//        REQUIRE( 1 == 1 );
//    }
}


TEST_CASE ("This is another case which fails", "[fail]")
{
    REQUIRE ( 1 == 1 );
}
