//
// Created by Jan Drewniok on 06.02.23.
//

#ifndef QUICKSIM_SIQAD_PLUGIN_TIMER_H
#define QUICKSIM_SIQAD_PLUGIN_TIMER_H

#include "logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace saglobal
{

using HDClock = std::chrono::high_resolution_clock;

class Stopwatch
{

  public:
    //! Constructor.
    Stopwatch() = default;

    //! Start or resume timer.
    void start()
    {
        start_count++;
        t_start       = HDClock::now();
        c_start       = std::clock();
        timer_running = true;
    }

    //! End or pause timer.
    void end()
    {
        if (!timer_running)
            throw std::invalid_argument("Attempted to end timer before it started.");
        t_accum += std::chrono::duration<double, std::milli>(HDClock::now() - t_start).count();
        c_accum += std::clock() - c_start;
        timer_running = false;
    }

    void printStopwatch(int const& log_level) const
    {
        std::stringstream ss;

        ss << std::fixed << std::setprecision(2) << std::endl;

        // Wall time
        ss << "\tWall time: " << t_accum << " ms";
        if (start_count > 1)
        {
            ss << " across " << start_count << " runs, avg " << t_accum / start_count << " ms";
        }
        ss << std::endl;

        // CPU time
        double cpu_time_ms = 1000.0 * static_cast<double>(c_accum) / CLOCKS_PER_SEC;
        ss << "\tCPU time: " << cpu_time_ms << " ms";
        if (start_count > 1)
        {
            ss << " across " << start_count << " runs, avg " << cpu_time_ms / start_count << " ms";
        }
        ss << std::endl;

        Logger log(log_level);
        log.echo() << ss.str() << std::endl;
    }

  private:
    // VARIABLES
    int                 start_count{};    //! Start/resume count for calculating average time.
    double              t_accum{};        //! Accumulated wall time.
    std::clock_t        c_accum{};        //! Accumulated CPU time.
    HDClock::time_point t_start{};        //! Last start/resume wall time.
    std::clock_t        c_start{};        //! Last start/resume CPU time.
    bool                timer_running{};  //! Indicate whether the timer is running.
};

}  // namespace saglobal

#endif  // QUICKSIM_SIQAD_PLUGIN_TIMER_H
