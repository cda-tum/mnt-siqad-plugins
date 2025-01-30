//
// Created by Willem Lambooy on 30.01.25.
//

#if (FICTION_ALGLIB_ENABLED)

#include "siqad_plugin_interface.hpp"
#include "timer.hpp"

#include <fmt/format.h>

#include <cstdlib>
#include <iostream>
#include <string_view>
#include <vector>

int main(int argc, char* argv[])
{
    try
    {
        std::cout << "ClusterComplete invoked" << std::endl;

        std::vector<std::string_view> cml_args;
        cml_args = std::vector<std::string_view>(argv, argv + argc);

        std::cout << "*** Argument Parsing ***" << std::endl;

        if (argc < 3)
        {
            std::cerr << "Error: Too few arguments. Expected input and output file names.\n";

            return EXIT_FAILURE;
        }

        std::string_view if_name;
        std::string_view of_name;

        if_name = cml_args[1];
        of_name = cml_args[2];

        bool verbose   = false;
        auto log_level = logger::DBG;

        for (const auto& arg : cml_args)
        {
            if (arg == "--debug")
            {
                // show additional debug information
                std::cout << "--debug: Showing additional outputs." << std::endl;
                verbose   = true;
                log_level = logger::DBG;
            }
            else
            {
                std::cout << fmt::format("Unrecognized command-line argument: {}", arg) << std::endl;
            }
        }

        logger log{log_level};

        simglobal::stopwatch stopwatch;

        log.echo() << "In File: " << if_name << std::endl;
        log.echo() << "Out File: " << of_name << std::endl;

        log.echo() << "\n*** Initiate ClusterComplete interface ***" << std::endl;
        log.echo() << "\n*** Read Simulation parameters ***" << std::endl;
        auto clustercomplete_interface =
            siqad_plugin_interface{if_name, of_name, verbose, log_level, fiction::sidb_simulation_engine::CLUSTERCOMPLETE};

        if (clustercomplete_interface.get_auto_fail() < clustercomplete_interface.get_cell_num())
        {
            log.warning() << "Problem size > autofail threshold, exiting." << std::endl;
            return EXIT_FAILURE;
        }

        log.echo() << "\n*** Invoke simulation ***" << std::endl;
        stopwatch.start();
        clustercomplete_interface.run_simulation();
        stopwatch.end();

        log.echo() << "\n*** Write simulation results ***" << std::endl;
        clustercomplete_interface.write_simulation_results();

        log.echo() << "\n*** ClusterComplete Complete ***" << std::endl;

        stopwatch.print_stopwatch(log_level);

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
#else  // FICTION_ALGLIB_ENABLED

#include <iostream>

int main()
{
    std::cout << "Usage of ALGLIB is not enabled; pass \"-DFICTION_ALGLIB\" to your CMake build." << std::endl;
    return EXIT_FAILURE;
}

#endif  // FICTION_ALGLIB_ENABLED
