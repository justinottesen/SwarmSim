#pragma once

#include <random>
#include <vector>

#include "Agent.h"
#include "Contract.h"
#include "Logger.h"

class Simulator {
 public:
  Simulator(const Params& params = {});

  void run();                   // Run simulation beginning to end
  void step(unsigned int t);    // Run single simulation step

 private:
  Params m_params;

  std::mt19937 m_rng;

  AgentManager    m_agentManager;
  ContractManager m_contractManager;
};
