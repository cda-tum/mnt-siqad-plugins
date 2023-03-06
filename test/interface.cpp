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
    auto qs_interface = quicksim_interface{fmt::format("{}/sim_problem_0.xml", TEST_PATH),
                                           fmt::format("{}/sim_result_0.xml", TEST_PATH), false};

    CHECK(qs_interface.get_quicksim_params().phys_params.lambda_tf == 5);
    CHECK(qs_interface.get_quicksim_params().phys_params.epsilon_r == 5.6);
    CHECK(qs_interface.get_quicksim_params().phys_params.mu == -.25);
    CHECK(qs_interface.get_quicksim_params().interation_steps == 70);
    CHECK(qs_interface.get_quicksim_params().alpha == 0.8);
    CHECK(qs_interface.get_quicksim_params().number_threads == std::thread::hardware_concurrency());

    CHECK(qs_interface.run_simulation() == 0);
    CHECK(!qs_interface.get_simulation_results().valid_lyts.empty());
    CHECK(qs_interface.get_simulation_results().valid_lyts[0].num_cells() == 7);

    CHECK(qs_interface.get_simulation_results().valid_lyts[0].get_cell_type({60, 23, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().valid_lyts[0].get_cell_type({58, 20, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().valid_lyts[0].get_cell_type({63, 21, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().valid_lyts[0].get_cell_type({57, 24, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().valid_lyts[0].get_cell_type({56, 23, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().valid_lyts[0].get_cell_type({53, 21, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().valid_lyts[0].get_cell_type({50, 24, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);

    qs_interface.write_sim_results();

    CHECK(std::filesystem::exists(fmt::format("{}/sim_result_0.xml", TEST_PATH)));
}
