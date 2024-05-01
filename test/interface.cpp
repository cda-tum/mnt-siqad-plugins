//
// Created by Jan Drewniok on 31.01.23.
//

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "logger.hpp"
#include "siqad_plugin_interface.hpp"

#include <fiction/algorithms/simulation/sidb/quickexact.hpp>
#include <fiction/algorithms/simulation/sidb/sidb_simulation_engine.hpp>
#include <fiction/traits.hpp>
#include <fiction/types.hpp>

#include <fmt/format.h>

#include <filesystem>
#include <thread>

TEST_CASE("Test if reading, simulating, and creating a result-file works for QuickSim", "[interface]")
{
    auto qs_interface = siqad_plugin_interface{fmt::format("{}/quicksim_problem_0.xml", TEST_PATH),
                                               fmt::format("{}/sim_result_0.xml", TEST_PATH), false, logger::MSG,
                                               fiction::sidb_simulation_engine::QUICKSIM};

    CHECK_THAT(qs_interface.get_physical_params().lambda_tf,
               Catch::Matchers::WithinAbs(5, fiction::physical_constants::POP_STABILITY_ERR));
    CHECK(qs_interface.get_quicksim_params().simulation_parameters.epsilon_r == 5.6);
    CHECK(qs_interface.get_quicksim_params().simulation_parameters.mu_minus == -.25);
    CHECK(qs_interface.get_quicksim_params().iteration_steps == 70);
    CHECK(qs_interface.get_quicksim_params().alpha == 0.8);
    CHECK(qs_interface.get_quicksim_params().number_threads == 1);

    REQUIRE(qs_interface.run_simulation() == 0);
    REQUIRE(!qs_interface.get_simulation_results().charge_distributions.empty());

    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({60, 23, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({58, 20, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({63, 21, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({57, 24, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({56, 23, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({53, 21, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({50, 24, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);

    qs_interface.write_simulation_results();

    CHECK(std::filesystem::exists(fmt::format("{}/sim_result_0.xml", TEST_PATH)));
}

TEST_CASE("Test if reading, simulating, and creating a result-file works for QuickExact", "[interface]")
{
    auto       qs_interface      = siqad_plugin_interface{fmt::format("{}/quickexact_problem_0.xml", TEST_PATH),
                                               fmt::format("{}/sim_result_0.xml", TEST_PATH), false, logger::MSG,
                                               fiction::sidb_simulation_engine::QUICKEXACT};
    const auto quickexact_params = qs_interface.get_quickexact_params();

    CHECK(quickexact_params.simulation_parameters.base == 2);
    CHECK_THAT(quickexact_params.simulation_parameters.lambda_tf,
               Catch::Matchers::WithinAbs(5, fiction::physical_constants::POP_STABILITY_ERR));
    CHECK(quickexact_params.simulation_parameters.epsilon_r == 5.6);
    CHECK(quickexact_params.simulation_parameters.mu_minus == -.32);

    CHECK(quickexact_params.base_number_detection ==
          fiction::quickexact_params<
              fiction::cell<fiction::sidb_cell_clk_lyt_siqad>>::automatic_base_number_detection::OFF);

    REQUIRE(qs_interface.run_simulation() == 0);
    REQUIRE(!qs_interface.get_simulation_results().charge_distributions.empty());

    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({60, 23, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({58, 20, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({63, 21, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({57, 24, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({56, 23, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({53, 21, 1}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
    CHECK(qs_interface.get_simulation_results().charge_distributions[0].get_cell_type({50, 24, 0}) ==
          fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);

    qs_interface.write_simulation_results();

    CHECK(std::filesystem::exists(fmt::format("{}/sim_result_0.xml", TEST_PATH)));
}
