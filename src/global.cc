// @file:     global.cc
// @author:   Samuel
// @created:  2019.02.26
// @license:  Apache License 2.0
//
// @desc:     Implementation of SimAnneal global functions.

#include "global.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace saglobal;

void TimeKeeper::printStopwatch(const std::string &key)
{
  Stopwatch *sw = stopwatches[key];
  std::stringstream ss;

  ss << std::fixed << std::setprecision(2) << key << std::endl;

  // Wall time
  double wall_time_ms = sw->wallTime();
  ss << "\tWall time: "
     << wall_time_ms << " ms";
  if (sw->startCount() > 1) {
    ss << " across " << sw->startCount() << " runs, avg " 
       << wall_time_ms / sw->startCount() << " ms";
  }
  ss << std::endl;

  // CPU time
  double cpu_time_ms = 1000.0 * sw->CPUTime() / CLOCKS_PER_SEC;
  ss << "\tCPU time: "
     << cpu_time_ms << " ms";
  if (sw->startCount() > 1) {
    ss << " across " << sw->startCount() << " runs, avg " 
       << cpu_time_ms / sw->startCount() << " ms";
  }
  ss << std::endl;

  Logger log(saglobal::log_level);
  log.echo() << ss.str() << std::endl;
}

void TimeKeeper::printAllStopwatches()
{
  for (std::string key : key_insertion_order)
    printStopwatch(key);
}
