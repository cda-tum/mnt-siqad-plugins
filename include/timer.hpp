//
// Created by Jan Drewniok on 06.02.23.
//

#ifndef MNT_SIQAD_PLUGINS_TIMER_HPP
#define MNT_SIQAD_PLUGINS_TIMER_HPP

#include "logger.hpp"

#include <fmt/format.h>

#include <chrono>
#include <ctime>
#include <exception>
#include <iomanip>
#include <map>
#include <ratio>
#include <sstream>
#include <string>
#include <vector>

namespace simglobal
{

using hidh_res_clock = std::chrono::high_resolution_clock;

class stopwatch
{

  public:
    //! Constructor.
    stopwatch() = default;

    //! Start or resume timer.
    void start()
    {
        start_count++;
        t_start       = hidh_res_clock::now();
        c_start       = std::clock();
        timer_running = true;
    }

    //! End or pause timer.
    void end()
    {
        if (!timer_running)
        {
            throw std::invalid_argument("Attempted to end timer before it started.");
        }

        t_accum += std::chrono::duration<double, std::milli>(hidh_res_clock::now() - t_start).count();
        c_accum += std::clock() - c_start;
        timer_running = false;
    }

    void print_stopwatch(const int log_level) const
    {
        std::stringstream ss;

        ss << std::fixed << std::setprecision(2) << std::endl;

        // Wall time
        ss << fmt::format("\tWall time: {} ms", t_accum);
        if (start_count > 1)
        {
            ss << fmt::format("across {} runs, avg {} ms", start_count, t_accum / start_count);
        }
        ss << std::endl;

        // CPU time
        double const cpu_time_ms = 1000.0 * static_cast<double>(c_accum) / CLOCKS_PER_SEC;

        ss << fmt::format("\tCPU time: {} ms", cpu_time_ms);
        if (start_count > 1)
        {
            ss << fmt::format(" across {} runs, avg {} ms", start_count, cpu_time_ms / start_count);
        }
        ss << std::endl;

        logger log(log_level);
        log.echo() << ss.str() << std::endl;
    }

  private:
    // VARIABLES
    int                        start_count{};    //! Start/resume count for calculating average time.
    double                     t_accum{};        //! Accumulated wall time.
    std::clock_t               c_accum{};        //! Accumulated CPU time.
    hidh_res_clock::time_point t_start{};        //! Last start/resume wall time.
    std::clock_t               c_start{};        //! Last start/resume CPU time.
    bool                       timer_running{};  //! Indicate whether the timer is running.
};

}  // namespace simglobal

#endif  // MNT_SIQAD_PLUGINS_TIMER_HPP
