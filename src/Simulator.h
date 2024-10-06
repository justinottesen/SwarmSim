#pragma once

#include <random>
#include <vector>

#include "Agent.h"
#include "Config.h"
#include "Contract.h"
#include "Logger.h"
#include "Metrics.h"

class Simulator {
 public:
  Simulator();

  void run();                   // Run simulation beginning to end
  void step(unsigned int t);    // Run single simulation step

 private:
  ConfigManager m_configManager;
  
  ConfigView<SimulationConfig> m_config;
  std::mt19937 m_rng;
  MetricManager   m_metricManager;

  AgentManager    m_agentManager;
  ContractManager m_contractManager;


};
