//
// Created by Jan Drewniok on 31.01.23.
//

#include "timer.h"
#include "interface.h"
#include <unordered_map>
#include <iostream>
#include <string>

using namespace phys;
using namespace saglobal;

int main(int argc, char *argv[])
{
    std::cout << "Physeng invoked" << std::endl;
    std::string if_name;
    std::string of_name;
    std::string ext_pots_name;
    std::vector<std::string> cml_args;

    std::cout << "*** Argument Parsing ***" << std::endl;

    if (argc < 3) {
        throw std::invalid_argument("Less arguments than excepted.");
    } else {
        // argv[0] is the binary
        if_name = argv[1];
        of_name = argv[2];

        // store the rest of the arguments
        for (int i=3; i<argc; i++) {
            cml_args.emplace_back(argv[i]);
        }
    }

    // parse additional arguments
    int ext_pots_step=0;
    bool verbose=false;
    unsigned long cml_i=0;
    int log_level = Logger::MSG;
    while (cml_i < cml_args.size()) {
        if (cml_args[cml_i] == "--debug") {
            // show additional debug information
            std::cout << "--debug: Showing additional outputs." << std::endl;
            verbose = true;
            log_level = Logger::DBG;
        } else {
            throw std::invalid_argument("Unrecognized command-line argument" + cml_args[cml_i]);
        }
        cml_i++;
    }

    Logger log(log_level);


    Stopwatch stopwatch;

    log.echo() << "In File: " << if_name << std::endl;
    log.echo() << "Out File: " << of_name << std::endl;
    log.echo() << "External Potentials File: " << ext_pots_name << std::endl;

    log.echo() << "\n*** Initiate QuickSim interface ***" << std::endl;
    log.echo() << "\n*** Read Simulation parameters ***" << std::endl;
    QuickSimInterface interface(if_name, of_name, ext_pots_name, ext_pots_step, verbose);

    log.echo() << "\n*** Invoke simulation ***" << std::endl;
    stopwatch.start();
    interface.runSimulation();
    stopwatch.end();

    log.echo() << "\n*** Write simulation results ***" << std::endl;
    interface.writeSimResults();

    log.echo() << "\n*** Quicksim Complete ***" << std::endl;

    stopwatch.printStopwatch(log_level);

    return EXIT_SUCCESS;
}

