#include "Logger.h"
#include "Simulator.h"

#include "TimeUtil.h"
#include <exception>

int main() {
  Logger::addConsole(DEBUG);
  Logger::addFile("log/info.log");
  Logger::addFile("log/trace.log", TRACE);

  std::filesystem::path data_dir("data");
  data_dir / program_time(true);
  Logger::addFile(data_dir / "info.log");

  LOG(INFO) << "Data & Log files will be stored at " << data_dir;

  try {
    Simulator sim;
    sim.run();
  } catch (const std::exception& e) {
    LOG(CRITICAL) << "Exception generated: " << e.what();
  } catch (...) {
    LOG(CRITICAL) << "Thrown value encountered, stopping execution";
    throw;
  }
}
