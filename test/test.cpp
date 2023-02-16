#include <catch2/catch_template_test_macros.hpp>

#include "interface.h"


TEST_CASE("Test", "[QuickSimInterface]")
{
    QuickSimInterface testclass{"test", "test", "test", 5};
    testclass.runSimulation();
}
