//
// Created by Jan Drewniok on 31.01.23.
//

#include "interface.hpp"

#include <catch2/catch_template_test_macros.hpp>

#include <fiction/types.hpp>

#include <fmt/format.h>

#include <filesystem>
#include <thread>

TEST_CASE("Test if reading, simulating, and creating a result-file works", "[interface]")
{
    auto qs_interface = quickexact_interface{fmt::format("{}/sim_problem_0.xml", TEST_PATH),
                                             fmt::format("{}/sim_result_0.xml", TEST_PATH), false};

    CHECK(qs_interface.get_physical_params().lambda_tf == 5);
    CHECK(qs_interface.get_physical_params().epsilon_r == 5.6);
    CHECK(qs_interface.get_physical_params().mu_minus == -.25);
    CHECK(qs_interface.get_quickexact_params().base_number_detection == fiction::automatic_base_number_detection::ON);

    REQUIRE(qs_interface.run_simulation() == 0);
    REQUIRE(!qs_interface.get_simulation_results().charge_distributions.empty());

    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({50, 24, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({51, 24, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({52, 24, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({53, 24, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);

    qs_interface.write_sim_results();

    CHECK(std::filesystem::exists(fmt::format("{}/sim_result_0.xml", TEST_PATH)));
}
