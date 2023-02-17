#include <catch2/catch_template_test_macros.hpp>

#include "interface.h"

const std::string file = fmt::format("{}/sim_problem_0.xml", EXPERIMENTS_PATH);

TEST_CASE("Test", "[QuickSimInterface]")
{
    quick_sim_interface interface{file, "sim_result_0.xml", false};
    CHECK(interface.get_quicksim_params().phys_params.lambda_tf == 5);
}
