//
// Created by Jan Drewniok on 31.01.23.
//

#ifndef QUICKSIM_SIQAD_PLUGIN_INTERFACE_H
#define QUICKSIM_SIQAD_PLUGIN_INTERFACE_H

#include "libs/siqadconn/src/siqadconn.h"
#include "logger.h"
#include <fiction/layouts/cell_level_layout.hpp>
#include <fiction/layouts/charge_distribution_layout.hpp>
#include <fiction/types.hpp>
#include <fiction/algorithms/simulation_sidb/sidb_simulation_parameters.hpp>

#include <string>

namespace phys {

    class QuickSimInterface {
    public:
        //! Constructure for SimAnnealInterface. Set defer_var_loading to true if
        //! you don't want simulation parameters to be loaded immediately from
        //! SiQADConn.
        QuickSimInterface(std::string t_in_path, std::string t_out_path,
                          std::string t_ext_pots_path, int t_ext_pots_step, bool verbose = false)
                : in_path(t_in_path), out_path(t_out_path), ext_pots_path(t_ext_pots_path),
                  ext_pots_step(t_ext_pots_step) {
            sqconn = new SiQADConnector(std::string("QuickSim"), in_path, out_path, verbose);
            initialize_chargelyt();
        }


        void runSimulation() {
            quicksim_stats <TestType> quicksimstats{};
            quicksim<TestType>(charge_layout, sim_params, &quicksimstats);
            sim_result = quicksimstats;
        }

        void writeSimResults(bool only_suggested_gs, bool qubo_energy) {
            // create the vector of strings for the db locations
            auto dbl_data = charge_layout.get_all_sidb_location_in_nm();
            for (unsigned int i = 0; i < dbl_data.size(); i++) { //need the index
                dbl_data[i].first = std::to_string(dbl_data[i].first);
                dbl_data[i].second = std::to_string(Sdbl_data[i].second);
            }
            std::map<double, uint64_t> energy_occ_distribution = energy_distribution(sim_results.valid_lyts);
            std::vector<std::vector<std::string>> db_dist_data;


            for (auto &it: energy_occ_distribution) {
                for (const auto &lyt: sim_results.valid_lyts) {
                    if (std::abs(it.first - lyt.get_system_energy()) / std::abs(lyt.get_system_energy()) < 0.00001) {
                        std::vector<std::string> db_dist;
                        const ExportElecConfigResult &result = result_it->second;
                        db_dist.push_back(charge_configuration_to_string(
                                lyt.get_all_sidb_charges()));                      // config
                        db_dist.push_back(std::to_string(lyt.get_system_energy()));  // energy
                        db_dist.push_back(std::to_string(it->second));      // occurance freq
                        db_dist.push_back(std::to_string(true));  // metastability
                        db_dist.push_back("2");                                   // 2-state
                        db_dist_data.push_back(db_dist);
                    }
                }
            }
            sqconn->setExport("db_charge", db_dist_data);
            sqconn->writeResultsXml();
        }


    private:

        void initialize_chargelyt() {
            sidb_cell_clk_lyt_siqad lyt{};
            Logger log(saglobal::log_level);


            // grab all physical locations
            log.debug() << "Grab all physical locations..." << std::endl;
            std::vector<std::pair<double, double>> db_locs;
            for (auto db: *(sqconn->dbCollection())) {
                db_locs.push_back(SimParams::latToEuclCoord(db->n, db->m, db->l));
                lyt.assign_cell_type({db->n, db->m, db->l}, TestType::cell_type::NORMAL);
                log.debug() << "DB loc: x=" << db_locs.back().first
                            << ", y=" << db_locs.back().second << std::endl;
            }

            sidb_simulation_parameters params{2};

            // variables: physical
            params.mu = std::stod(sqconn->getParameter("muzm"));
            params.epsilon_r = std::stod(sqconn->getParameter("eps_r"));
            params.lambda_tf = std::stod(sqconn->getParameter("debye_length"));
            log.echo() << "Retrieval from SiQADConn complete." << std::endl;
            sim_param{quicksim_params{params}};
            charge_layout{charge_ly{lyt}};
        }

        // Instances
        SiQADConnector *sqconn = nullptr;

        // variables
        std::string in_path;
        std::string out_path;
        std::string ext_pots_path;
        int ext_pots_step;
        charge_distribution_surface charge_layout{};
        quicksim_stats sim_results{};
        quicksim_params sim_params{};
    };
}

#endif //QUICKSIM_SIQAD_PLUGIN_INTERFACE_H