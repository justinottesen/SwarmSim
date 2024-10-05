#include "TimeUtil.h"

#include <sys/time.h>

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string current_time(bool path) {
  timeval tv;
  gettimeofday(&tv, nullptr);
  tm                 tm = *std::localtime(&tv.tv_sec);
  std::ostringstream ss;
  ss << std::put_time(&tm, "%F %T") << "." << std::setw(3) << std::setfill('0')
     << tv.tv_usec / 1000;
  std::string date_str = ss.str();
  if (path) {
    std::ranges::replace(date_str, ' ', '_');
    std::ranges::replace(date_str, ':', '-');
  }
  return date_str;
}

std::string program_time(bool path) {
  static std::string time_str = current_time();
  if (path) {
    std::string copy = time_str;
    std::ranges::replace(copy, ' ', '_');
    std::ranges::replace(copy, ':', '-');
    return copy;
  }
  return time_str;
}
