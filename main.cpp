#include "Logger.h"
#include "Simulator.h"


int main() {
  Logger::addConsole(TRACE);
  Logger::addFile("log/info.log");
  Logger::addFile("log/trace.log", TRACE);

  Simulator sim;

  return sim.run();

}