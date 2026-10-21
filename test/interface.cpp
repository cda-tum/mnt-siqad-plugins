//
// Created by Jan Drewniok on 31.01.23.
//

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "logger.hpp"
#include "siqad_plugin_interface.hpp"

#include <fiction/technology/sidb/simulation/engine.hpp>
#include <fiction/technology/sidb/simulation/engines/quickexact.hpp>
#include <fiction/utils/math/math_utils.hpp>

#include <fmt/format.h>

#include <filesystem>
#include <thread>

/**
 * Checks that XML locations and charge states share the result layout's raster order.
 * @param sim_interface Simulated plugin interface whose XML file has been written.
 */
void check_simulation_xml(const siqad_plugin_interface& sim_interface)
{
    const auto                  result = sim_interface.get_simulation_results();
    boost::property_tree::ptree xml{};
    boost::property_tree::read_xml(fmt::format("{}/sim_result_0.xml", TEST_PATH), xml);
    const auto& locations = xml.get_child("sim_out.physloc");
    REQUIRE(locations.size() == result.lyt.num_dots());
    std::size_t i = 0;
    for (const auto& [name, location] : locations)
    {
        CHECK(name == "dbdot");
        const auto [x, y] = result.lyt.get_lattice().nm_position(result.lyt.sidbs()[i++]);
        CHECK_THAT(location.get<double>("<xmlattr>.x"), Catch::Matchers::WithinAbs(x * 10, 0.000001));
        CHECK_THAT(location.get<double>("<xmlattr>.y"), Catch::Matchers::WithinAbs(y * 10, 0.000001));
    }
    std::map<std::string, double> expected{};
    for (const auto& distribution : result.charge_distributions)
    {
        CHECK(distribution.sites() == result.lyt.sidbs());
        expected.emplace(fiction::sidb::model::charge_configuration_to_string(distribution.charge_states()),
                         distribution.energy());
    }
    const auto& distributions = xml.get_child("sim_out.elec_dist");
    CHECK(distributions.size() == expected.size());
    for (const auto& [name, distribution] : distributions)
    {
        CHECK(name == "dist");
        const auto found = expected.find(distribution.data());
        REQUIRE(found != expected.end());
        CHECK_THAT(distribution.get<double>("<xmlattr>.energy"), Catch::Matchers::WithinAbs(found->second, 0.000001));
        CHECK(distribution.get<int>("<xmlattr>.state_count") == 3);
        expected.erase(found);
    }
    CHECK(expected.empty());
}

TEST_CASE("Test if reading, simulating, and creating a result-file works for QuickSim", "[interface]")
{
    auto sim_interface = siqad_plugin_interface{fmt::format("{}/quicksim_problem_0.xml", TEST_PATH),
                                                fmt::format("{}/sim_result_0.xml", TEST_PATH), false, logger::MSG,
                                                fiction::sidb::simulation::engine::QUICKSIM};

    const auto quicksim_params = sim_interface.get_quicksim_params();

    CHECK_THAT(quicksim_params.sim_params.lambda_tf, Catch::Matchers::WithinAbs(5, fiction::utils::math::ERROR_MARGIN));
    CHECK(quicksim_params.sim_params.epsilon_r == 5.6);
    CHECK(quicksim_params.sim_params.mu_minus == -.25);
    CHECK(quicksim_params.iteration_steps == 70);
    CHECK(quicksim_params.alpha == 0.8);
    CHECK(quicksim_params.number_threads == 1);

    REQUIRE(sim_interface.run_simulation() == 0);
    REQUIRE(!sim_interface.get_simulation_results().charge_distributions.empty());

    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({60, 23, 0}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({58, 20, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({63, 21, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({57, 24, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({56, 23, 0}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({53, 21, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({50, 24, 0}) == fiction::sidb::dot_tag::NORMAL);

    sim_interface.write_simulation_results();
    check_simulation_xml(sim_interface);

    CHECK(std::filesystem::exists(fmt::format("{}/sim_result_0.xml", TEST_PATH)));
}

TEST_CASE("Test if reading, simulating, and creating a result-file works for QuickExact", "[interface]")
{
    auto sim_interface = siqad_plugin_interface{fmt::format("{}/quickexact_problem_0.xml", TEST_PATH),
                                                fmt::format("{}/sim_result_0.xml", TEST_PATH), false, logger::MSG,
                                                fiction::sidb::simulation::engine::QUICKEXACT};

    const auto quickexact_params = sim_interface.get_quickexact_params();

    CHECK(quickexact_params.sim_params.base == 2);
    CHECK_THAT(quickexact_params.sim_params.lambda_tf,
               Catch::Matchers::WithinAbs(5, fiction::utils::math::ERROR_MARGIN));
    CHECK(quickexact_params.sim_params.epsilon_r == 5.6);
    CHECK(quickexact_params.sim_params.mu_minus == -.32);

    CHECK(quickexact_params.base_number_detection ==
          fiction::sidb::simulation::engines::quickexact_params::automatic_base_number_detection::OFF);
    CHECK_THAT(quickexact_params.global_potential, Catch::Matchers::WithinAbs(0, fiction::utils::math::ERROR_MARGIN));

    REQUIRE(sim_interface.run_simulation() == 0);
    REQUIRE(!sim_interface.get_simulation_results().charge_distributions.empty());

    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({60, 23, 0}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({58, 20, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({63, 21, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({57, 24, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({56, 23, 0}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({53, 21, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({50, 24, 0}) == fiction::sidb::dot_tag::NORMAL);

    sim_interface.write_simulation_results();
    check_simulation_xml(sim_interface);

    CHECK(std::filesystem::exists(fmt::format("{}/sim_result_0.xml", TEST_PATH)));
}

#if (FICTION_ALGLIB_ENABLED)
TEST_CASE("Test if reading, simulating, and creating a result-file works for ClusterComplete", "[interface]")
{
    auto sim_interface = siqad_plugin_interface{fmt::format("{}/clustercomplete_problem_0.xml", TEST_PATH),
                                                fmt::format("{}/sim_result_0.xml", TEST_PATH), false, logger::MSG,
                                                fiction::sidb::simulation::engine::CLUSTERCOMPLETE};

    const auto clustercomplete_params = sim_interface.get_clustercomplete_params();

    CHECK(clustercomplete_params.sim_params.base == 2);
    CHECK_THAT(clustercomplete_params.sim_params.lambda_tf,
               Catch::Matchers::WithinAbs(5, fiction::utils::math::ERROR_MARGIN));
    CHECK(clustercomplete_params.sim_params.epsilon_r == 5.6);
    CHECK(clustercomplete_params.sim_params.mu_minus == -.32);

    CHECK(clustercomplete_params.validity_witness_partitioning_max_cluster_size_gss == 10);
    CHECK(clustercomplete_params.num_overlapping_witnesses_limit_gss == 8);
    CHECK_THAT(clustercomplete_params.global_potential,
               Catch::Matchers::WithinAbs(0, fiction::utils::math::ERROR_MARGIN));
    CHECK(clustercomplete_params.report_gss_stats ==
          fiction::sidb::simulation::engines::clustercomplete_params::ground_state_space_reporting::ON);

    REQUIRE(sim_interface.run_simulation() == 0);
    REQUIRE(!sim_interface.get_simulation_results().charge_distributions.empty());

    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({60, 23, 0}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({58, 20, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({63, 21, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({57, 24, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({56, 23, 0}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({53, 21, 1}) == fiction::sidb::dot_tag::NORMAL);
    CHECK(sim_interface.get_simulation_results().lyt.get_dot_tag({50, 24, 0}) == fiction::sidb::dot_tag::NORMAL);

    sim_interface.write_simulation_results();
    check_simulation_xml(sim_interface);

    CHECK(std::filesystem::exists(fmt::format("{}/sim_result_0.xml", TEST_PATH)));
}

#endif  // FICTION_ALGLIB_ENABLED

TEST_CASE("QuickSim reports an exhausted search", "[interface]")
{
    auto sim_interface = siqad_plugin_interface{fmt::format("{}/quicksim_problem_0.xml", TEST_PATH),
                                                fmt::format("{}/sim_result_0.xml", TEST_PATH), false, logger::MSG,
                                                fiction::sidb::simulation::engine::QUICKSIM};
    REQUIRE(sim_interface.run_simulation() == EXIT_SUCCESS);
    sim_interface.get_quicksim_params().iteration_steps = 0;
    CHECK(sim_interface.run_simulation() == EXIT_FAILURE);
    CHECK_THROWS_AS(sim_interface.write_simulation_results(), std::runtime_error);
}

TEST_CASE("Export preserves distinct charge states beyond 64 SiDBs", "[interface]")
{
    boost::property_tree::ptree input{};
    boost::property_tree::read_xml(fmt::format("{}/quickexact_problem_0.xml", TEST_PATH), input);
    for (auto& [name, layer] : input.get_child("siqad.design"))
    {
        if (name == "layer" && layer.get<std::string>("<xmlattr>.type", "") == "DB")
        {
            layer.erase("dbdot");
            for (int i = 0; i < 66; ++i)
            {
                boost::property_tree::ptree dot{};
                dot.put("latcoord.<xmlattr>.n", i < 2 ? i : i * 1000);
                dot.put("latcoord.<xmlattr>.m", 0);
                dot.put("latcoord.<xmlattr>.l", 0);
                layer.add_child("dbdot", dot);
            }
        }
    }
    const auto input_path = fmt::format("{}/large_problem.xml", TEST_PATH);
    boost::property_tree::write_xml(input_path, input);
    auto sim_interface =
        siqad_plugin_interface{input_path, fmt::format("{}/sim_result_0.xml", TEST_PATH), false, logger::MSG};
    std::filesystem::remove(input_path);
    REQUIRE(sim_interface.run_simulation() == EXIT_SUCCESS);
    REQUIRE(sim_interface.get_simulation_results().charge_distributions.size() == 2);
    sim_interface.write_simulation_results();
    check_simulation_xml(sim_interface);
}
