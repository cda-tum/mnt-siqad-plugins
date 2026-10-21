//
// Created by Willem Lambooy on 30.01.25.
//

#if (FICTION_ALGLIB_ENABLED)

#include "logger.hpp"
#include "siqad_plugin_interface.hpp"
#include "timer.hpp"

#include <fiction/technology/sidb/simulation/engine.hpp>

#include <fmt/format.h>

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <span>
#include <string_view>

/**
 * @brief Runs ClusterComplete on a SiQAD problem file.
 * @param argc Number of command-line arguments.
 * @param argv Program name, input and output paths, and optional flags.
 * @return EXIT_SUCCESS when simulation results are written, otherwise EXIT_FAILURE.
 */
int main(int argc, char* argv[])
{
    try
    {
        std::cout << "ClusterComplete invoked" << '\n';

        const std::span cml_args{argv, static_cast<std::size_t>(argc)};

        std::cout << "*** Argument Parsing ***" << '\n';

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

        for (const std::string_view arg : cml_args.subspan(3))
        {
            if (arg == "--debug")
            {
                // show additional debug information
                std::cout << "--debug: Showing additional outputs." << '\n';
                verbose   = true;
                log_level = logger::DBG;
            }
            else
            {
                std::cout << fmt::format("Unrecognized command-line argument: {}", arg) << '\n';
            }
        }

        logger log{log_level};

        simglobal::stopwatch stopwatch;

        log.echo() << "In File: " << if_name << '\n';
        log.echo() << "Out File: " << of_name << '\n';

        log.echo() << "\n*** Initiate ClusterComplete interface ***" << '\n';
        log.echo() << "\n*** Read Simulation parameters ***" << '\n';
        auto clustercomplete_interface = siqad_plugin_interface{if_name, of_name, verbose, log_level,
                                                                fiction::sidb::simulation::engine::CLUSTERCOMPLETE};

        if (clustercomplete_interface.get_auto_fail() < clustercomplete_interface.get_cell_num())
        {
            log.warning() << "Problem size > autofail threshold, exiting." << '\n';
            return EXIT_FAILURE;
        }

        log.echo() << "\n*** Invoke simulation ***" << '\n';
        stopwatch.start();
        if (clustercomplete_interface.run_simulation() != EXIT_SUCCESS)
        {
            log.critical() << "Simulation produced no charge distributions." << '\n';
            return EXIT_FAILURE;
        }
        stopwatch.end();

        log.echo() << "\n*** Write simulation results ***" << '\n';
        clustercomplete_interface.write_simulation_results();

        log.echo() << "\n*** ClusterComplete Finished ***" << '\n';

        stopwatch.print_stopwatch(log_level);

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
#else  // FICTION_ALGLIB_ENABLED

#include <cstdlib>
#include <iostream>

/**
 * @brief Reports that ClusterComplete requires ALGLIB.
 * @return EXIT_FAILURE.
 */
int main()
{
    std::cout << "Usage of ALGLIB is not enabled; pass \"-DFICTION_ALGLIB\" to your CMake build." << '\n';
    return EXIT_FAILURE;
}

#endif  // FICTION_ALGLIB_ENABLED
