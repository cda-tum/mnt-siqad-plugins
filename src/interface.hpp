//
// Created by Jan Drewniok on 31.01.23.
//

#ifndef QUICKSIM_SIQAD_PLUGIN_INTERFACE_HPP
#define QUICKSIM_SIQAD_PLUGIN_INTERFACE_HPP

#include "logger.hpp"
#include "siqadconn.cc"
#include "siqadconn.h"

#include <fiction/algorithms/simulation/sidb/quicksim.hpp>
#include <fiction/algorithms/simulation/sidb/sidb_simulation_parameters.hpp>
#include <fiction/layouts/cell_level_layout.hpp>
#include <fiction/technology/charge_distribution_surface.hpp>
#include <fiction/traits.hpp>
#include <fiction/types.hpp>

#include <fmt/format.h>

#include <cmath>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

class quicksim_interface
{
  public:
    //! Constructor for QuickSimInterface
    quicksim_interface(const std::string_view& t_in_path, const std::string_view& t_out_path, const bool verbose = true,
                       const int log_l = logger::MSG) :
            log_level{log_l},
            in_path{t_in_path},
            out_path{t_out_path}
    {
        sqconn = std::make_unique<SiQADConnector>("QuickSim", static_cast<std::string>(in_path),
                                                  static_cast<std::string>(out_path), verbose);
        initialize_fiction_layout();
    }

    int run_simulation()
    {
        fiction::quicksim<fiction::sidb_cell_clk_lyt_siqad>(layout, sim_par, &sim_results);

        return EXIT_SUCCESS;
    }

    void write_sim_results()
    {
        // create the vector of strings for the db locations
        const auto data = sim_results.valid_lyts[0].get_all_sidb_location_in_nm();

        std::vector<std::pair<std::string, std::string>> dbl_data{};
        dbl_data.reserve(data.size());

        for (const auto& d : data)
        {
            dbl_data.emplace_back(std::to_string(d.first * 10E9), std::to_string(d.second * 10E9));
        }

        sqconn->setExport("db_loc", dbl_data);

        std::vector<std::vector<std::string>> db_dist_data{};
        db_dist_data.reserve(sim_results.valid_lyts.size());

        for (const auto& [energy, deg] : fiction::energy_distribution(sim_results.valid_lyts))
        {
            for (const auto& lyt : sim_results.valid_lyts)
            {
                if (std::abs(energy - lyt.get_system_energy()) < fiction::physical_constants::POP_STABILITY_ERR)
                {
                    db_dist_data.push_back({{
                        fiction::charge_configuration_to_string(lyt.get_all_sidb_charges()),  // config
                        std::to_string(lyt.get_system_energy()),                              // energy
                        std::to_string(deg),                                                  // occurrence freq
                        "1",                                                                  // metastability
                        "3"  // 2-state (GUI still does not work for 2)
                    }});

                    break;
                }
            }
        }

        sqconn->setExport("db_charge", db_dist_data);

        sqconn->writeResultsXml();
    }

    [[nodiscard]] fiction::quicksim_params& get_quicksim_params() noexcept
    {
        return sim_par;
    }

    [[nodiscard]] fiction::quicksim_stats<fiction::sidb_cell_clk_lyt_siqad> get_simulation_results() const noexcept
    {
        return sim_results;
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

        fiction::sidb_simulation_parameters params{2};

        try
        {
            // variables: physical
            params.mu        = std::stod(sqconn->getParameter("muzm"));
            params.epsilon_r = std::round(std::stod(sqconn->getParameter("eps_r")) * 100) / 100;  // round to two digits
            params.lambda_tf = std::stod(sqconn->getParameter("debye_length"));

            const auto iteration_steps = static_cast<uint64_t>(std::stoi(sqconn->getParameter("iteration_steps")));
            const auto alpha           = std::stod(sqconn->getParameter("alpha"));
            
            
            // Check if number_threads is negative
            if (const auto number_threads = static_cast<int64_t>(std::stod(sqconn->getParameter("num_threads")));
                number_threads >= 0)
            {
                // Update sim_par with number_threads
                sim_par.number_threads = static_cast<uint64_t>(number_threads);
            }

            log.echo() << "Retrieval from SiQADConn complete." << std::endl;

            sim_par = fiction::quicksim_params{params, iteration_steps, alpha};
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
    const int                                                 log_level;
    const std::string_view                                    in_path;
    const std::string_view                                    out_path;
    fiction::sidb_cell_clk_lyt_siqad                          layout{};
    fiction::quicksim_params                                  sim_par{};
    fiction::quicksim_stats<fiction::sidb_cell_clk_lyt_siqad> sim_results{};
};

#endif  // QUICKSIM_SIQAD_PLUGIN_INTERFACE_HPP
