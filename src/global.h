// @file:     global.h
// @author:   Samuel
// @created:  2019.02.26
// @license:  Apache License 2.0
//
// @desc:     Global functions for SimAnneal

#ifndef _SAGLOBAL_H_
#define _SAGLOBAL_H_

#include <map>
#include <string>
#include <chrono>
#include <ctime>
#include <vector>

#include "logger.h"

typedef double FPType;

namespace saglobal {

  class Stopwatch;
  class TimeKeeper;

  typedef std::chrono::high_resolution_clock HDClock;
  typedef std::chrono::duration<double, std::milli> HDClockDuration;
  typedef std::map<std::string, Stopwatch*> StopwatchMap;

  // default log level
  extern int log_level;

  //! Timer class referenced by user classes to start and end timers.
  class Stopwatch
  {
  public:

    //! Constructor.
    Stopwatch() : start_count(0), t_accum(0), c_accum(0) {};

    //! Destructor.
    ~Stopwatch() {};

    //! Start or resume timer.
    void start()
    {
      start_count++;
      t_start = HDClock::now(); 
      c_start = std::clock(); 
      timer_running = true;
    }

    //! End or pause timer.
    void end()
    {
      if (!timer_running)
        throw "Attempted to end timer before it started.";
      t_accum += HDClockDuration(HDClock::now() - t_start).count();
      c_accum += std::clock() - c_start;
      timer_running = false;
    }

    //! Return the number of times the stopwatch has been started/resumed.
    int startCount() {return start_count;}

    //! Return the accumulated wall time in a duration object counted in 
    //! milliseconds.
    double wallTime() {return t_accum;}

    //! Return the accumulated CPU time.
    double CPUTime() {return c_accum;}

  private:

    // VARIABLES
    int start_count;              //! Start/resume count for calculating average time.
    double t_accum;               //! Accumulated wall time.
    std::clock_t c_accum;         //! Accumulated CPU time.
    HDClock::time_point t_start;  //! Last start/resume wall time.
    std::clock_t c_start;         //! Last start/resume CPU time.
    bool timer_running;           //! Indicate whether the timer is running.

  };

  //! Singleton class which keeps track of all times, referenced by ID.
  class TimeKeeper
  {
  public:

    //! Constructor.
    TimeKeeper() {};

    //! Destructor.
    ~TimeKeeper() {
      for (auto sw : stopwatches)
        delete sw.second;
    }

    //! Return the singleton instance of the TimeKeeper.
    static TimeKeeper *instance()
    {
      if (time_keeper == nullptr)
        time_keeper = new TimeKeeper();
      return time_keeper;
    }

    //! Create a stopwatch corresponding to the key string and return a pointer 
    //! to the stopwatch.
    Stopwatch *createStopwatch(const std::string &key)
    {
      Stopwatch *sw = new Stopwatch();
      stopwatches.insert(StopwatchMap::value_type(key, sw));
      key_insertion_order.push_back(key);
      return sw;
    }

    //! Return a reference to the stopwatch corresponding to the key string.
    Stopwatch *getStopwatch(const std::string &key) {return stopwatches.at(key);}

    //! Print the specified stopwatch results.
    void printStopwatch(const std::string &key);

    //! Print all stopwatch results.
    void printAllStopwatches();

  private:

    static TimeKeeper *time_keeper;
    std::vector<std::string> key_insertion_order;
    StopwatchMap stopwatches;

  };


}

#endif
