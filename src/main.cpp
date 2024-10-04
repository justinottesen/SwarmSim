#include "Logger.h"
#include "Simulator.h"

int main() {
  Logger::addConsole(INFO);
  Logger::addFile("log/info.log");
  Logger::addFile("log/trace.log", TRACE);

  Simulator sim;

  sim.run();
}
