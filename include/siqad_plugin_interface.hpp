//
// Created by Jan Drewniok on 31.01.23.
//

#ifndef QUICKSIM_SIQAD_PLUGIN_INTERFACE_HPP
#define QUICKSIM_SIQAD_PLUGIN_INTERFACE_HPP

#include "logger.hpp"
#include "siqadconn.cc"
#include "siqadconn.h"

#include <fiction/algorithms/simulation/sidb/enum_class_exhaustive_algorithm.hpp>
#include <fiction/algorithms/simulation/sidb/quickexact.hpp>
#include <fiction/algorithms/simulation/sidb/quicksim.hpp>
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
    //! Constructor for QuickSimInterface
    siqad_plugin_interface(const std::string_view& t_in_path, const std::string_view& t_out_path,
                           const bool verbose = true, const int log_l = logger::MSG,
                           fiction::exhaustive_algorithm engine = fiction::exhaustive_algorithm::QUICKEXACT) :
            log_level{log_l},
            in_path{t_in_path},
            out_path{t_out_path},
            simulation_engine{engine}
    {
        sqconn = std::make_unique<SiQADConnector>("QuickExact", static_cast<std::string>(in_path),
                                                  static_cast<std::string>(out_path), verbose);
        initialize_fiction_layout();
    }

    int run_simulation()
    {
        if (simulation_engine == fiction::exhaustive_algorithm::QUICKEXACT)
        {
            sim_results = fiction::quickexact<fiction::sidb_cell_clk_lyt_siqad>(layout, params_all);
        }
        else if (simulation_engine == fiction::exhaustive_algorithm::EXGS)
        {
            while (sim_results.charge_distributions.empty())
            {
                sim_results = fiction::quicksim<fiction::sidb_cell_clk_lyt_siqad>(layout, sim_par);
            }
        }

        return EXIT_SUCCESS;
    }

    void write_sim_results()
    {
        // create the vector of strings for the db locations
        const auto data = sim_results.charge_distributions.front().get_all_sidb_locations_in_nm();

        std::vector<std::pair<std::string, std::string>> dbl_data{};
        dbl_data.reserve(data.size());

        for (const auto& d : data)
        {
            dbl_data.emplace_back(std::to_string(d.first * 10), std::to_string(d.second * 10));
        }

        sqconn->setExport("db_loc", dbl_data);

        std::vector<std::vector<std::string>> db_dist_data{};
        db_dist_data.reserve(sim_results.charge_distributions.size());

        for (const auto& lyt : sim_results.charge_distributions)
        {
            db_dist_data.push_back({{
                fiction::charge_configuration_to_string(lyt.get_all_sidb_charges()),  // config
                std::to_string(lyt.get_system_energy()),                              // energy
                std::to_string(1),                                                    // occurrence freq
                "1",                                                                  // metastability
                "3"  // 3-state (GUI still does not work for 2)
            }});
        }

        sqconn->setExport("db_charge", db_dist_data);

        sqconn->writeResultsXml();
    }

    [[nodiscard]] fiction::sidb_simulation_parameters& get_physical_params() noexcept
    {
        return params_all.physical_parameters;
    }

    [[nodiscard]] fiction::quickexact_params<fiction::sidb_cell_clk_lyt_siqad>& get_quickexact_params() noexcept
    {
        return params_all;
    }

    [[nodiscard]] fiction::quicksim_params& get_quicksim_params() noexcept
    {
        return sim_par;
    }

    [[nodiscard]] fiction::sidb_simulation_result<fiction::sidb_cell_clk_lyt_siqad>
    get_simulation_results() const noexcept
    {
        return sim_results;
    }

    [[nodiscard]] uint64_t get_auto_fail() const
    {
        return auto_fail;
    }

    [[nodiscard]] uint64_t get_cell_num() const
    {
        return layout.num_cells();
    }

  private:
    void initialize_fiction_layout()
    {
        logger log(log_level);

        // grab all physical locations
        log.debug() << "Grab all physical locations..." << std::endl;

        auto* const db_collection = sqconn->dbCollection();

        std::vector<std::pair<double, double>> db_locs{};
        db_locs.reserve(db_collection->db_tree_inner->size());

        for (const auto& db : *db_collection)
        {
            db_locs.push_back(fiction::sidb_nm_position<fiction::sidb_cell_clk_lyt_siqad>(
                fiction::sidb_simulation_parameters{}, {db->n, db->m, db->l}));

            layout.assign_cell_type({db->n, db->m, db->l}, fiction::sidb_cell_clk_lyt_siqad::cell_type::NORMAL);

            log.debug() << fmt::format("DB loc: x={}, y={}", db_locs.back().first, db_locs.back().second) << std::endl;
        }

        try
        {
            // variables: physical
            fiction::sidb_simulation_parameters params{};
            params.mu_minus = std::stod(sqconn->getParameter("muzm"));

            params.epsilon_r =
                fiction::round_to_n_decimal_places(std::stod(sqconn->getParameter("eps_r")), 2);  // round to two digits
            params.lambda_tf = std::stod(sqconn->getParameter("debye_length"));

            auto_fail = std::stoi(sqconn->getParameter("autofail"));

            if (simulation_engine == fiction::exhaustive_algorithm::QUICKEXACT)
            {
                params.base                    = static_cast<uint8_t>(std::stoi(sqconn->getParameter("base_number")));
                params_all.physical_parameters = params;
                if (std::stoi(sqconn->getParameter("autodetection")) == 1)
                {
                    params_all.base_number_detection = fiction::automatic_base_number_detection::ON;
                }
                else
                {
                    params_all.base_number_detection = fiction::automatic_base_number_detection::OFF;
                }
            }
            else if (simulation_engine == fiction::exhaustive_algorithm::EXGS)
            {
                const auto iteration_steps = static_cast<uint64_t>(std::stoi(sqconn->getParameter("iteration_steps")));
                const auto alpha           = std::stod(sqconn->getParameter("alpha"));

                // prevent number of threads to be negative
                if (const auto number_threads = static_cast<int64_t>(std::stod(sqconn->getParameter("num_threads")));
                    number_threads >= 0)
                {
                    // Update sim_par with number_threads
                    sim_par.number_threads = static_cast<uint64_t>(number_threads);
                }
                sim_par = fiction::quicksim_params{params, iteration_steps, alpha};
            }
            log.echo() << "Retrieval from SiQADConn complete." << std::endl;
        }
        catch (...)
        {
            log.critical() << "Could not retrieve all parameters from SiQADConn." << std::endl;
            throw;
        }
    }

    // Instances
    std::unique_ptr<SiQADConnector> sqconn = nullptr;

    // variables
    uint64_t                                                          auto_fail;
    const int                                                         log_level;
    const std::string_view                                            in_path;
    const std::string_view                                            out_path;
    fiction::sidb_cell_clk_lyt_siqad                                  layout{};
    fiction::sidb_simulation_result<fiction::sidb_cell_clk_lyt_siqad> sim_results{};
    fiction::quickexact_params<fiction::sidb_cell_clk_lyt_siqad>      params_all{};
    fiction::quicksim_params                                          sim_par{};
    fiction::exhaustive_algorithm                                     simulation_engine{};
};

#endif  // QUICKSIM_SIQAD_PLUGIN_INTERFACE_HPP