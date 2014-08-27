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

    {
        std::string str;
        for (std::size_t i = 0; i < arg.getOperandCount(); ++i)
        {
            str += arg.getOperand(i);
        }
        REQUIRE ( str == "folder/filenamesystem-pcontroldata" );
    }

    ttl::Argument arg2;
    arg2.pass("a derp \"herp merp\"");
    REQUIRE ( arg2.getOperandCount() == 2 );
}


TEST_CASE ("Testing file2str", "[file2str]")
{
    REQUIRE ( ttl::file2str("../../rsc/test_file.txt") == "test\ntext" );
}


TEST_CASE ("Trying to sleep", "[sleep]")
{
    ttl::Sti_t now = std::time(nullptr);
    ttl::msleep(1100);
    now -= std::time(nullptr);
    now = -now;
    REQUIRE ( now >= 1 );
}


TEST_CASE ("Rit binary distribution", "[rit]")
{
    ttl::Rit rit;
    enum {first, second} which = first;
    for (int i = 0; i < 30; ++i)
    {
        if (rit.isFirstReady())
        {
            REQUIRE ( which == first );
            which = second;
        }
        else if (rit.isSecondReady())
        {
            REQUIRE ( which == second );
            which = first;
        }
    }
}



