//
// Created by Jan Drewniok on 31.01.23.
//

#ifndef MNT_SIQAD_PLUGINS_SIQAD_PLUGIN_INTERFACE_HPP
#define MNT_SIQAD_PLUGINS_SIQAD_PLUGIN_INTERFACE_HPP

#include "logger.hpp"
#include "siqadconn.cc"
#include "siqadconn.h"

#include <fiction/algorithms/simulation/sidb/clustercomplete.hpp>
#include <fiction/algorithms/simulation/sidb/quickexact.hpp>
#include <fiction/algorithms/simulation/sidb/quicksim.hpp>
#include <fiction/algorithms/simulation/sidb/sidb_simulation_engine.hpp>
#include <fiction/algorithms/simulation/sidb/sidb_simulation_parameters.hpp>
#include <fiction/layouts/cell_level_layout.hpp>
#include <fiction/technology/charge_distribution_surface.hpp>
#include <fiction/traits.hpp>
#include <fiction/types.hpp>
#include <fiction/utils/math_utils.hpp>

#include <fmt/format.h>

#include <cmath>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

class siqad_plugin_interface
{
  public:
    //! Constructor for siqad_plugin_interface
    siqad_plugin_interface(const std::string_view& t_in_path, const std::string_view& t_out_path,
                           const bool verbose = true, const int log_l = logger::MSG,
                           fiction::sidb_simulation_engine engine = fiction::sidb_simulation_engine::QUICKEXACT) :
            log_level{log_l},
            in_path{t_in_path},
            out_path{t_out_path},
            simulation_engine{engine}
    {
        sqconn = std::make_unique<SiQADConnector>("", static_cast<std::string>(in_path),
                                                  static_cast<std::string>(out_path), verbose);
        initialize_fiction_layout();
    }

    // conduct physical simulation for given layout
    int run_simulation()
    {
        if (simulation_engine == fiction::sidb_simulation_engine::QUICKEXACT)
        {
            simulation_results = fiction::quickexact<fiction::sidb_cell_clk_lyt_siqad>(layout, quickexact_params);
        }
        else if (simulation_engine == fiction::sidb_simulation_engine::QUICKSIM)
        {
            std::size_t invocations = 0;
            while (simulation_results.charge_distributions.empty() && invocations < 100)
            {
                invocations += 1;
                simulation_results = fiction::quicksim<fiction::sidb_cell_clk_lyt_siqad>(layout, quicksim_params);
            }
        }
#if (FICTION_ALGLIB_ENABLED)
        else if (simulation_engine == fiction::sidb_simulation_engine::CLUSTERCOMPLETE)
        {
            simulation_results =
                fiction::clustercomplete<fiction::sidb_cell_clk_lyt_siqad>(layout, clustercomplete_params);
        }
#endif  // FICTION_ALGLIB_ENABLED

        return EXIT_SUCCESS;
    }

    // simulation results are written to xml
    void write_simulation_results()
    {
        // create the vector of strings for the db locations
        const auto data = simulation_results.charge_distributions.front().get_all_sidb_locations_in_nm();

        std::vector<std::pair<std::string, std::string>> dbl_data{};
        dbl_data.reserve(data.size());

        for (const auto& [x, y] : data)
        {
            dbl_data.emplace_back(std::to_string(x * 10), std::to_string(y * 10));
        }

        sqconn->setExport("db_loc", dbl_data);

        std::vector<std::vector<std::string>> db_dist_data{};
        db_dist_data.reserve(simulation_results.charge_distributions.size());

        std::set<uint64_t> unique_index{};
        for (const auto& lyt : simulation_results.charge_distributions)
        {
            lyt.charge_distribution_to_index_general();
            unique_index.insert(lyt.get_charge_index_and_base().first);
        }

        for (const auto& index : unique_index)
        {
            for (const auto& lyt : simulation_results.charge_distributions)
            {
                lyt.charge_distribution_to_index_general();
                if (lyt.get_charge_index_and_base().first == index)
                {
                    db_dist_data.push_back({{
                        fiction::charge_configuration_to_string(lyt.get_all_sidb_charges()),  // config
                        std::to_string(lyt.get_system_energy()),                              // energy
                        std::to_string(1),                                                    // occurrence freq
                        "1",                                                                  // metastability
                        "3"  // 3-state (GUI still does not work for 2)
                    }});
                    break;
                }
            }
        }

        sqconn->setExport("db_charge", db_dist_data);

        sqconn->writeResultsXml();
    }
#if (FICTION_ALGLIB_ENABLED)
    // get the clustercomplete parameter
    [[nodiscard]] fiction::clustercomplete_params<fiction::cell<fiction::sidb_cell_clk_lyt_siqad>>&
    get_clustercomplete_params() noexcept
    {
        return clustercomplete_params;
    }
#endif  // FICTION_ALGLIB_ENABLED
    // get the quickexact parameter
    [[nodiscard]] fiction::quickexact_params<fiction::cell<fiction::sidb_cell_clk_lyt_siqad>>&
    get_quickexact_params() noexcept
    {
        return quickexact_params;
    }
    // get the quicksim parameter
    [[nodiscard]] fiction::quicksim_params& get_quicksim_params() noexcept
    {
        return quicksim_params;
    }
    // get the simulation results
    [[nodiscard]] fiction::sidb_simulation_result<fiction::sidb_cell_clk_lyt_siqad>
    get_simulation_results() const noexcept
    {
        return simulation_results;
    }
    // get number of SiDBs set at auto fail
    [[nodiscard]] uint64_t get_auto_fail() const
    {
        return auto_fail;
    }
    // get the number of cells of the given layout
    [[nodiscard]] uint64_t get_cell_num() const
    {
        return layout.num_cells();
    }

  private:
    // parameters are initialized and prepared for the simulation
    void initialize_fiction_layout()
    {
        logger log(log_level);

        // grab all physical locations
        log.debug() << "Grab all physical locations..." << '\n';

        auto* const db_collection = sqconn->dbCollection();

        std::vector<std::pair<double, double>> db_locs{};
        db_locs.reserve(static_cast<uint64_t>(static_cast<double>(db_collection->db_tree_inner->size())));

        for (const auto& db : *db_collection)
        {
            db_locs.push_back(fiction::sidb_nm_position<fiction::sidb_cell_clk_lyt_siqad>(
                fiction::sidb_cell_clk_lyt_siqad{}, {db->n, db->m, db->l}));

            layout.assign_cell_type({db->n, db->m, db->l}, fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);

            log.debug() << fmt::format("DB loc: x={}, y={}\n", db_locs.back().first, db_locs.back().second);
        }

        try
        {
            // variables: physical
            fiction::sidb_simulation_parameters params{};
            params.mu_minus = std::stod(sqconn->getParameter("muzm"));

            params.epsilon_r =
                fiction::round_to_n_decimal_places(std::stod(sqconn->getParameter("eps_r")), 2);  // round to two digits
            params.lambda_tf = std::stod(sqconn->getParameter("debye_length"));

            auto_fail = static_cast<uint64_t>(std::stoi(sqconn->getParameter("autofail")));

            if (simulation_engine == fiction::sidb_simulation_engine::QUICKEXACT)
            {
                params.base = static_cast<uint8_t>(std::stoi(sqconn->getParameter("base_number")));
                quickexact_params.simulation_parameters = params;
                quickexact_params.global_potential      = fiction::round_to_n_decimal_places(
                    std::stod(sqconn->getParameter("glob_pot")), 6);  // round to six digits
                if (std::stoi(sqconn->getParameter("autodetection")) == 1)
                {
                    quickexact_params.base_number_detection = fiction::quickexact_params<
                        fiction::cell<fiction::sidb_cell_clk_lyt_siqad>>::automatic_base_number_detection::ON;
                }
                else
                {
                    quickexact_params.base_number_detection = fiction::quickexact_params<
                        fiction::cell<fiction::sidb_cell_clk_lyt_siqad>>::automatic_base_number_detection::OFF;
                }
            }
            else if (simulation_engine == fiction::sidb_simulation_engine::QUICKSIM)
            {
                const auto iteration_steps = static_cast<uint64_t>(std::stoi(sqconn->getParameter("iteration_steps")));
                const auto alpha           = std::stod(sqconn->getParameter("alpha"));
                quicksim_params            = fiction::quicksim_params{params, iteration_steps, alpha};

                // prevent number of threads to be negative
                if (const auto number_threads = static_cast<int64_t>(std::stod(sqconn->getParameter("num_threads")));
                    number_threads >= 0)
                {
                    // Update quicksim_params with number_threads
                    quicksim_params.number_threads = static_cast<uint64_t>(number_threads);
                }
            }
#if (FICTION_ALGLIB_ENABLED)
            else if (simulation_engine == fiction::sidb_simulation_engine::CLUSTERCOMPLETE)
            {
                params.base = static_cast<uint8_t>(std::stoi(sqconn->getParameter("base_number")));
                clustercomplete_params.simulation_parameters = params;
                clustercomplete_params.global_potential      = fiction::round_to_n_decimal_places(
                    std::stod(sqconn->getParameter("glob_pot")), 6);  // round to six digits
                clustercomplete_params.validity_witness_partitioning_max_cluster_size_gss =
                    static_cast<uint64_t>(std::stoi(sqconn->getParameter("witness_partitioning_limit")));
                clustercomplete_params.num_overlapping_witnesses_limit_gss =
                    static_cast<uint64_t>(std::stoi(sqconn->getParameter("overlapping_witnesses_limit")));
                if (std::stoi(sqconn->getParameter("presim_stats")) == 1)
                {
                    clustercomplete_params.report_gss_stats = fiction::clustercomplete_params<
                        fiction::cell<fiction::sidb_cell_clk_lyt_siqad>>::ground_state_space_reporting::ON;
                }
                else
                {
                    clustercomplete_params.report_gss_stats = fiction::clustercomplete_params<
                        fiction::cell<fiction::sidb_cell_clk_lyt_siqad>>::ground_state_space_reporting::OFF;
                }

                // prevent number of threads to be negative
                if (const auto number_threads = static_cast<int64_t>(std::stod(sqconn->getParameter("num_threads")));
                    number_threads >= 0)
                {
                    // Update clustercomplete_params with number_threads
                    clustercomplete_params.available_threads = static_cast<uint64_t>(number_threads);
                }
            }
#endif  // FICTION_ALGLIB_ENABLED
            log.echo() << "Retrieval from SiQADConn complete." << '\n';
        }
        catch (...)
        {
            log.critical() << "Could not retrieve all parameters from SiQADConn." << '\n';
            throw;
        }
    }

    // Instances
    std::unique_ptr<SiQADConnector> sqconn = nullptr;

    // variables
    uint64_t                                                          auto_fail{};
    const int                                                         log_level{};
    const std::string_view                                            in_path;
    const std::string_view                                            out_path;
    fiction::sidb_cell_clk_lyt_siqad                                  layout;
    fiction::sidb_simulation_result<fiction::sidb_cell_clk_lyt_siqad> simulation_results;
#if (FICTION_ALGLIB_ENABLED)
    fiction::clustercomplete_params<fiction::cell<fiction::sidb_cell_clk_lyt_siqad>> clustercomplete_params{};
#endif  // FICTION_ALGLIB_ENABLED
    fiction::quickexact_params<fiction::cell<fiction::sidb_cell_clk_lyt_siqad>> quickexact_params{};
    fiction::quicksim_params                                                    quicksim_params{};
    fiction::sidb_simulation_engine                                             simulation_engine{};
};

#endif  // MNT_SIQAD_PLUGINS_SIQAD_PLUGIN_INTERFACE_HPP
