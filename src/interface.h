//
// Created by Jan Drewniok on 31.01.23.
//

#ifndef QUICKSIM_SIQAD_PLUGIN_INTERFACE_H
#define QUICKSIM_SIQAD_PLUGIN_INTERFACE_H

#include "siqadconn.h"
#include "siqadconn.cc"
#include "logger.h"
#include <fiction/layouts/cell_level_layout.hpp>
#include <fiction/technology/charge_distribution_surface.hpp>
#include <fiction/types.hpp>
#include <fiction/traits.hpp>
#include <fiction/algorithms/simulation/sidb/sidb_simulation_parameters.hpp>
#include <fiction/algorithms/simulation/sidb/quicksim.hpp>
#include <string>
#include <utility>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace fiction;
using namespace phys;

class QuickSimInterface {
public:
    //! Constructor for QuickSimInterface
    QuickSimInterface(std::string t_in_path, std::string t_out_path,
                      std::string t_ext_pots_path, int t_ext_pots_step, bool verbose = true,
                      int log_l = Logger::MSG)
            : log_level{log_l}, in_path(std::move(t_in_path)), out_path(std::move(t_out_path)),
              ext_pots_path(std::move(t_ext_pots_path)) {
        sqconn = new SiQADConnector(std::string("QuickSim"), in_path, out_path, verbose);
        initialize_layout_fiction();
    }

    ~QuickSimInterface() {
        delete sqconn;
    }

    int runSimulation() {
        quicksim<sidb_cell_clk_lyt_siqad>(layout, sim_par, &sim_results);
        return EXIT_SUCCESS;
    }

    void writeSimResults() {
        // create the vector of strings for the db locations
        auto data = sim_results.valid_lyts[0].get_all_sidb_location_in_nm();
        std::vector<std::pair<std::string, std::string>> dbl_data(data.size());
        for (unsigned int i = 0; i < data.size(); i++) { //need the index
            dbl_data[i].first = std::to_string(data[i].first * 10E9);
            dbl_data[i].second = std::to_string(data[i].second * 10E9);
        }

        sqconn->setExport("db_loc", dbl_data);

        std::map<double, uint64_t> energy_occ_distribution = energy_distribution(sim_results.valid_lyts);
        std::vector<std::vector<std::string>> db_dist_data;

        for (const auto &[energy, deg]: energy_occ_distribution) {
            for (const auto &lyt: sim_results.valid_lyts) {
                if (std::abs(energy - lyt.get_system_energy()) < physical_constants::POP_STABILITY_ERR) {
                    std::vector<std::string> db_dist;
                    db_dist.push_back(
                            charge_configuration_to_string(lyt.get_all_sidb_charges()));                      // config
                    db_dist.push_back(std::to_string(lyt.get_system_energy()));  // energy
                    db_dist.push_back(std::to_string(deg));      // occurance freq
                    db_dist.push_back(std::to_string(true));  // metastability
                    db_dist.emplace_back("3");             // 2-state (GUI still does not work for 2)
                    db_dist_data.push_back(db_dist);
                    break;
                }
            }
        }
        sqconn->setExport("db_charge", db_dist_data);
        sqconn->writeResultsXml();
    }


private:

    void initialize_layout_fiction() {
        Logger log(log_level);


        // grab all physical locations
        log.debug() << "Grab all physical locations..." << std::endl;
        std::vector<std::pair<double, double>> db_locs;
        for (auto db: *(sqconn->dbCollection())) {
            db_locs.push_back(
                    sidb_nm_position<sidb_cell_clk_lyt_siqad>(sidb_simulation_parameters{}, {db->n, db->m, db->l}));
            layout.assign_cell_type({db->n, db->m, db->l}, sidb_cell_clk_lyt_siqad::cell_type::NORMAL);
            log.debug() << "DB loc: x=" << db_locs.back().first
                        << ", y=" << db_locs.back().second << std::endl;
        }

        sidb_simulation_parameters params{2};

        // variables: physical
        params.mu = std::stod(sqconn->getParameter("muzm"));
        params.epsilon_r = std::stod(sqconn->getParameter("eps_r"));
        params.lambda_tf = std::stod(sqconn->getParameter("debye_length"));
        auto iteration_steps = static_cast<uint64_t>(std::stoi(sqconn->getParameter("iteration_steps")));
        auto alpha = std::stod(sqconn->getParameter("alpha"));
        log.echo() << "Retrieval from SiQADConn complete." << std::endl;
        sim_par = quicksim_params{params, iteration_steps, alpha};
    }

    // Instances
    SiQADConnector *sqconn = nullptr;

    // variables
    int log_level;
    std::string in_path;
    std::string out_path;
    std::string ext_pots_path;
    sidb_cell_clk_lyt_siqad layout{};
    quicksim_stats<sidb_cell_clk_lyt_siqad> sim_results{};
    quicksim_params sim_par{};
};

#endif //QUICKSIM_SIQAD_PLUGIN_INTERFACE_H