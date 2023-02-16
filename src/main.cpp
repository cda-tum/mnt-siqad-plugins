//
// Created by Jan Drewniok on 31.01.23.
//

#include "interface.h"
#include "timer.h"

#include <iostream>
#include <string>
#include <unordered_map>

using namespace phys;
using namespace saglobal;

int main(int argc, char* argv[])
{
    std::cout << "Physeng invoked" << std::endl;
    std::string              ext_pots_name;
    const std::vector<std::string> cml_args(argv, argv + argc);


    std::cout << "*** Argument Parsing ***" << std::endl;

    if (argc < 3)
    {
        std::cerr << "Error: Too few arguments. Expected input and output file names.\n";
        return 1;
    }

    const std::string if_name = argv[1];
    const std::string of_name = argv[2];


    int      ext_pots_step = 0;
    bool     verbose       = false;
    auto     log_level     = Logger::DBG;
    uint64_t cml_i         = 0;
    while (cml_i < cml_args.size())
    {
        if (cml_args[cml_i] == "--debug")
        {
            // show additional debug information
            std::cout << "--debug: Showing additional outputs." << std::endl;
            verbose   = true;
            log_level = Logger::DBG;
        }
        else
        {
            std::cout << "Unrecognized command-line argument: " + cml_args[cml_i] << std::endl;
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
