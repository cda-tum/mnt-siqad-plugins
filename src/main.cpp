//
// Created by Jan Drewniok on 31.01.23.
//

#include "global.h"
#include "interface.h"
#include <unordered_map>
#include <iostream>
#include <string>


using namespace phys;

int main(int argc, char *argv[])
{
    std::cout << "Physeng invoked" << std::endl;
    std::string if_name, of_name, ext_pots_name;
    std::vector<std::string> cml_args;

    std::cout << "*** Argument Parsing ***" << std::endl;

    if (argc < 3) {
        throw "Less arguments than excepted.";
    } else {
        // argv[0] is the binary
        if_name = argv[1];
        of_name = argv[2];

        // store the rest of the arguments
        for (int i=3; i<argc; i++) {
            cml_args.push_back(argv[i]);
        }
    }

    // parse additional arguments
    int ext_pots_step=0;
    bool only_suggested_gs=false;
    bool qubo_energy=false;
    bool verbose=false;
    unsigned long cml_i=0;
    while (cml_i < cml_args.size()) {
        if (cml_args[cml_i] == "--ext-pots") {
            std::cout << "--ext-pots: Import external potentials." << std::endl;
            ext_pots_name = cml_args[++cml_i];
        } else if (cml_args[cml_i] == "--ext-pots-step") {
            std::cout << "--ext-pots-step: Specify the step to use for potentials."
                      << std::endl;
            ext_pots_step = stoi(cml_args[++cml_i]);
        } else if (cml_args[cml_i] == "--only-suggested-gs") {
            // each SimAnneal instance only returns one configuration
            std::cout << "--only-suggested-gs: Only returning suggested ground state "
                      << "from each instance." << std::endl;
            only_suggested_gs = true;
        } else if (cml_args[cml_i] == "--debug") {
            // show additional debug information
            std::cout << "--debug: Showing additional outputs." << std::endl;
            verbose = true;
            saglobal::log_level = Logger::DBG;
        } else {
            throw "Unrecognized command-line argument: " + cml_args[cml_i];
        }
        cml_i++;
    }

    Logger log(saglobal::log_level);

    saglobal::TimeKeeper *tk = saglobal::TimeKeeper::instance();
    saglobal::Stopwatch *sw_simulation = tk->createStopwatch("Total Simulation");

    log.echo() << "In File: " << if_name << std::endl;
    log.echo() << "Out File: " << of_name << std::endl;
    log.echo() << "External Potentials File: " << ext_pots_name << std::endl;

    log.echo() << "\n*** Initiate SimAnneal interface ***" << std::endl;
    log.echo() << "\n*** Read Simulation parameters ***" << std::endl;
    QuickSimInterface interface(if_name, of_name, ext_pots_name, ext_pots_step, verbose);

    log.echo() << "\n*** Invoke simulation ***" << std::endl;
    sw_simulation->start();
    interface.runSimulation(sparams);
    sw_simulation->end();

    log.echo() << "\n*** Write simulation results ***" << std::endl;
    interface.writeSimResults(only_suggested_gs, qubo_energy);

    log.echo() << "\n*** SimAnneal Complete ***" << std::endl;

    tk->printAllStopwatches();

    delete tk;

    return 1;
}

